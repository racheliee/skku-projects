import argparse
import base64
import os
import random
import shutil
import subprocess
import time
import traceback
import uuid

from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import Select

# You can increase this if your server is very slow. 
SERVER_WAIT = 0.5


def image_to_data_url(image_path):
    """
    Convert an image to a data URL.
    """
    # Read the image file in binary mode
    with open(image_path, 'rb') as image_file:
        image_data = image_file.read()
        base64_encoded_data = base64.b64encode(image_data)
        base64_string = base64_encoded_data.decode('utf-8')
        mime_type = 'image/jpeg' if image_path.endswith('.jpg') else 'image/png'
        return f"data:{mime_type};base64,{base64_string}"


class StopGrading(Exception):
    pass

class py4web(object):
    
    def start_server(self, path_to_app, port=8400, debug=False):
        self.debug = debug
        print("Starting the server")
        self.port = port
        self.debug = debug
        self.app_name = os.path.basename(path_to_app)
        subprocess.run(
            "rm -rf /tmp/apps && mkdir -p /tmp/apps && echo '' > /tmp/apps/__init__.py",
            shell=True,
            check=True,
        )
        self.app_folder = os.path.join("/tmp/apps", self.app_name)
        shutil.copytree(path_to_app, self.app_folder)
        subprocess.run(["rm", "-rf", os.path.join(self.app_folder, "databases")])
        self.server = subprocess.Popen(
            [
                "py4web",
                "run",
                "/tmp/apps",
                "--port",
                str(self.port),
                "--app_names",
                self.app_name,
            ],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )
        started = False
        while True:
            self.server.stdout.flush()
            line = self.server.stdout.readline().decode().strip()
            if not line:
                continue
            print(line)
            if "[X]" in line:
                started = True
            if "127.0.0.1:" in line:
                if not started:
                    raise StopGrading
                print("- app started!")
                break
        if self.debug:
            self.browser = webdriver.Firefox()
        else:
            browser_options = webdriver.ChromeOptions()
            browser_options.add_argument("--headless")
            self.browser =  webdriver.Chrome(options=browser_options)
        
    def __del__(self):
        if self.server:
            self.stop_server()

    def stop_server(self):
        print("- stopping server...")
        self.server.kill()
        self.server = None
        print("- stopping server...DONE")
        if not self.debug:
            self.browser.quit()
            print("- browser stopped.")
        
    def goto(self, path):
        self.browser.get(f"http://127.0.0.1:{self.port}/{self.app_name}/{path}")
        self.browser.implicitly_wait(0.2)
        
    def refresh(self):
        self.browser.refresh()
        self.browser.implicitly_wait(0.2)
        
    def register_user(self, user):
        """Registers a user."""
        self.goto("auth/register")
        self.browser.find_element(By.NAME, "email").send_keys(user["email"])
        self.browser.find_element(By.NAME, "password").send_keys(user["password"])
        self.browser.find_element(By.NAME, "password_again").send_keys(user["password"])
        self.browser.find_element(By.NAME, "first_name").send_keys(user.get("first_name", ""))
        self.browser.find_element(By.NAME, "last_name").send_keys(user.get("last_name", ""))
        self.browser.find_element(By.CSS_SELECTOR, "input[type='submit']").click()
        
    def login(self, user):     
        self.goto("auth/login")
        self.browser.find_element(By.NAME, "email").send_keys(user["email"])
        self.browser.find_element(By.NAME, "password").send_keys(user["password"])
        self.browser.find_element(By.CSS_SELECTOR, "input[type='submit']").click()


class ProtoAssignment(py4web):
    
    def __init__(self, app_path, debug=False):
        super().__init__()
        self.start_server(app_path, debug=debug)
        self._comments = []
        self.user1 = self.get_user()
        self.user2 = self.get_user()
        self.user1['email'] = "admin@example.com"
        
    def get_user(self):
        return {
            "email": uuid.uuid4().hex + "@ucsc.edu",
            "password": str(uuid.uuid4()),
            "first_name": str(uuid.uuid4()),
            "last_name": str(uuid.uuid4()),
        }
    
    def append_comment(self, points, comment):
        self._comments.append((points, comment))
        
    def setup(self):
        self.register_user(self.user1)
        self.register_user(self.user2)
            
    def grade(self):
        self.setup()
        steps = [getattr(self, name) for name in dir(self) if name.startswith("step")]
        for step in steps:
            try:
                g, c = step()
                self.append_comment(g, step.__name__ + f": {g} point(s): {c}")
            except StopGrading:
                break
            except Exception as e:
                traceback.print_exc()
                self.append_comment(0, f"Error in {step.__name__}: {e}")
        grade = 0
        for points, comment in self._comments:
            print("=" * 40)
            print(f"[{points} points]", comment)
            grade += points
        print("=" * 40)
        print(f"TOTAL GRADE {grade}")
        print("=" * 40)
        self.stop_server()
        return grade


class Assignment(ProtoAssignment):
    
    def __init__(self, app_path, debug=False):
        super().__init__(os.path.join(app_path, "apps/contact_us"), debug=debug)
        self.key1 = str(uuid.uuid4())
        self.key2 = str(uuid.uuid4())
        self.key3 = str(uuid.uuid4())
        self.info1 = {
            "name": str(uuid.uuid4()) + self.key1,
            "email": "a@b.com",
            "phone": "123-456-7890",
            "message": str(uuid.uuid4()) + self.key1,
        }
        self.info2 = {
            "name": str(uuid.uuid4()) + self.key2,
            "email": "c@b.com",
            "phone": "111-222-3333",
            "message": str(uuid.uuid4()) + self.key3 
        }
        self.info3 = {
            "name": str(uuid.uuid4()) + self.key1,
            "email": "e@f.org",
            "phone": "111-222-3333",
            "message": str(uuid.uuid4()) + self.key3 
        }
        self.emails = {f["email"] for f in [self.info1, self.info2, self.info3]}

    def add_contact(self, c):
        self.goto('index')
        input_name = self.browser.find_element(By.CSS_SELECTOR, "input[name='name']")
        input_name.send_keys(c["name"])
        input_email = self.browser.find_element(By.CSS_SELECTOR, "input[name='email']")
        input_email.send_keys(c["email"])
        input_phone = self.browser.find_element(By.CSS_SELECTOR, "input[name='phone']")
        input_phone.send_keys(c["phone"])
        input_message = self.browser.find_element(By.CSS_SELECTOR, "textarea[name='message']")
        input_message.send_keys(c["message"])
        self.browser.find_element(By.CSS_SELECTOR, "input[type='submit']").click()
        return True
        
        
    def step1(self):
        """Adding contact information"""
        self.add_contact(self.info1)
        self.add_contact(self.info2)
        self.add_contact(self.info3)
        errors = self.browser.find_elements(By.CSS_SELECTOR, "p.py4web-validation-error")
        assert len(errors) == 0, "S1-1 There should be no errors."
        return 1, "Posts are added correctly."
    
    def step2(self):
        """Empty name or phone"""
        infoa = dict(self.info1)
        infoa["name"] = ""
        self.add_contact(infoa)
        assert self.browser.find_element(By.CSS_SELECTOR, "p.py4web-validation-error") is not None, "S2-1 Empty names should be rejected."
        infob = dict(self.info2)
        infob["phone"] = ""
        self.add_contact(infoa)
        assert self.browser.find_element(By.CSS_SELECTOR, "p.py4web-validation-error") is not None, "S2-2 Empty phones should be rejected."
        return 1, "Empty names and phones are rejected."   
    
    def step3(self):
        """No bad emails"""
        infoa = dict(self.info1)
        infoa["email"] = "hello@there"
        self.add_contact(infoa)
        assert self.browser.find_element(By.CSS_SELECTOR, "p.py4web-validation-error") is not None, "S3-1 Bad emails should be rejected."
        infob = dict(self.info2)
        infob["email"] = ""
        self.add_contact(infoa)
        assert self.browser.find_element(By.CSS_SELECTOR, "p.py4web-validation-error") is not None, "S3-2 Empty emails should be rejected."
        infob = dict(self.info2)
        infob["email"] = "@somewhere.com"
        self.add_contact(infoa)
        assert self.browser.find_element(By.CSS_SELECTOR, "p.py4web-validation-error") is not None, "S3-3 Bad emails should be rejected."
        return 1, "Empty or bad emails are rejected." 
    
    def step4(self):
        """Only admin@example.com can see the grid."""
        self.goto("contact_requests")
        grid = self.browser.find_elements(By.CSS_SELECTOR, "table.grid-table")
        assert len(grid) == 0, "S4-1 Non-logged in users should not be able to see the grid."
        form = self.browser.find_elements(By.CSS_SELECTOR, "form")
        assert len(form) == 1, "S4-2 Non-logged in users should be redirected to the index page."
        self.login(self.user2)
        self.goto("contact_requests")
        grid = self.browser.find_elements(By.CSS_SELECTOR, "table.grid-table")
        assert len(grid) == 0, "S4-3 Non-logged in users should not be able to see the grid."
        form = self.browser.find_elements(By.CSS_SELECTOR, "form")
        assert len(form) == 1, "S4-4 Non-logged in users should be redirected to the index page."
        self.login(self.user1)
        self.goto("contact_requests")
        grid = self.browser.find_elements(By.CSS_SELECTOR, "table.grid-table")
        assert len(grid) == 1, "S4-5 The admin@example.com user should be able to see the grid."
        return 1, "Only admin@example.com can access the grid."
             
    def step5(self):
        self.goto("contact_requests")
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_request_email")
        if len(email_cells) == 0:
            email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_requests_email")
        if len(email_cells) == 0:
            email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_request-email")
        if len(email_cells) == 0:
            email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_requests-email")
        emails1 = {e.text for e in email_cells}
        assert emails1 == self.emails, "S5-1 The emails are not correct in the grid."
        message_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_request_message")
        if len(message_cells) == 0:
            message_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_requests_message")
        if len(message_cells) == 0:
            message_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_request-message")
        if len(message_cells) == 0:
            message_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_requests-message")
        messages = {e.text for e in message_cells}
        correct_messages = {self.info1["message"], self.info2["message"], self.info3["message"]}
        assert messages == correct_messages, "S5-2 The messages are not correct in the grid."        
        return 1, "The grid contains the correct information."
    
    def step6(self):
        # First a successful search.
        self.goto("contact_requests")
        select = Select(self.browser.find_element(By.CSS_SELECTOR, "select.grid-search-form-input"))
        select.select_by_visible_text("Search by Name")
        inp = self.browser.find_element(By.CSS_SELECTOR, "input.grid-search-form-input")
        inp.send_keys(self.key1)
        self.browser.find_element(By.CSS_SELECTOR, "input.grid-search-button").click()
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_request_email")
        if len(email_cells) == 0:
            email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_requests_email")
        if len(email_cells) == 0:
            email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_request-email")        
        if len(email_cells) == 0:
            email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_requests-email")        
        emails = {e.text for e in email_cells}
        ok_emails = {f["email"] for f in [self.info1, self.info3]}
        assert emails == ok_emails, "S6-1 Search by name does not work."
        # Then an empty search. 
        select = Select(self.browser.find_element(By.CSS_SELECTOR, "select.grid-search-form-input"))
        select.select_by_visible_text("Search by Name")
        inp = self.browser.find_element(By.CSS_SELECTOR, "input.grid-search-form-input")
        inp.send_keys(str(uuid.uuid4()))
        self.browser.find_element(By.CSS_SELECTOR, "input.grid-search-button").click()
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_request_email")
        assert len(email_cells) == 0, "S6-12 Search by name returns spurious results."
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_requests_email")
        assert len(email_cells) == 0, "S6-12 Search by name returns spurious results."
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_request-email")        
        assert len(email_cells) == 0, "S6-12 Search by name returns spurious results."
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_requests-email")        
        assert len(email_cells) == 0, "S6-12 Search by name returns spurious results."
        return 1, "Search by name works."
        
    def step7(self):
        self.goto("contact_requests")
        # First a successful search.
        select = Select(self.browser.find_element(By.CSS_SELECTOR, "select.grid-search-form-input"))
        select.select_by_visible_text("Search by Message")
        inp = self.browser.find_element(By.CSS_SELECTOR, "input.grid-search-form-input")
        inp.send_keys(self.key3)
        self.browser.find_element(By.CSS_SELECTOR, "input.grid-search-button").click()
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_request_email")
        if len(email_cells) == 0:
            email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_requests_email")
        if len(email_cells) == 0:
            email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_request-email")      
        if len(email_cells) == 0:
            email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_requests-email")      
        emails = {e.text for e in email_cells}
        assert emails == {f["email"] for f in [self.info2, self.info3]}, "S7-1 Search by message does not work."
        # Then an empty search. 
        select = Select(self.browser.find_element(By.CSS_SELECTOR, "select.grid-search-form-input"))
        select.select_by_visible_text("Search by Message")
        inp = self.browser.find_element(By.CSS_SELECTOR, "input.grid-search-form-input")
        inp.send_keys(str(uuid.uuid4()))
        self.browser.find_element(By.CSS_SELECTOR, "input.grid-search-button").click()
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_request_email")
        assert len(email_cells) == 0, "S7-2 Search by message returns spurious results."
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_requests_email")
        assert len(email_cells) == 0, "S7-2 Search by message returns spurious results."
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_request-email")      
        assert len(email_cells) == 0, "S7-2 Search by message returns spurious results."
        email_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_requests-email")      
        assert len(email_cells) == 0, "S7-2 Search by message returns spurious results."
        return 1, "Search by message works."

    def step8(self):
        self.goto("contact_requests")
        for i in range(3):
            rows = self.browser.find_elements(By.CSS_SELECTOR, "tr[role='row']")
            name_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_request_name")
            if len(name_cells) == 0:
                name_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_requests_name")
            if len(name_cells) == 0:
                name_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_request-name")
            if len(name_cells) == 0:
                name_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_requests-name")
            names = [n.text for n in name_cells]
            k = random.randint(0, len(names) - 1)
            del_button = rows[k].find_element(By.CSS_SELECTOR, "a.grid-delete-button")
            del_button.click()
            self.browser.switch_to.alert.accept();
            rows = self.browser.find_elements(By.CSS_SELECTOR, "tr[role='row']")
            name_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_request_name")
            if len(name_cells) == 0:
                name_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-col-contact_requests_name")
            if len(name_cells) == 0:
                name_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_request-name")
            if len(name_cells) == 0:
                name_cells = self.browser.find_elements(By.CSS_SELECTOR, "td.grid-cell-contact_requests-name")
            remaining_names = [n.text for n in name_cells]
            names.pop(k)
            assert names == remaining_names, "S8-1 Deletion does not work."
        return 1, "Deletion works"
            
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--debug', default=False, action='store_true',
                        help="Debug mode (show browser).")

    tests = Assignment(".", debug=parser.parse_args().debug)
    tests.grade()
