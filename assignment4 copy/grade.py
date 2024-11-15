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
    
    def start_server(self, path_to_app, args=None):
        print("Starting the server")
        self.app_name = os.path.basename(path_to_app)
        subprocess.run(
            "rm -rf /tmp/apps && mkdir -p /tmp/apps && echo '' > /tmp/apps/__init__.py",
            shell=True,
            check=True,
        )
        self.app_folder = os.path.join("/tmp/apps", self.app_name)
        shutil.copytree(path_to_app, self.app_folder)
        subprocess.run(["rm", "-rf", os.path.join(self.app_folder, "databases")])
        shutil.rmtree("/tmp/test_images", ignore_errors=True)
        shutil.copytree("test_images", "/tmp/test_images")
        self.server = subprocess.Popen(
            [
                "py4web",
                "run",
                "/tmp/apps",
                "--port",
                str(args.port),
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
        if not args.debug:
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
        if not args.debug:
            self.browser.quit()
            print("- browser stopped.")
        
    def goto(self, path):
        self.browser.get(f"http://127.0.0.1:{args.port}/{self.app_name}/{path}")
        self.browser.implicitly_wait(SERVER_WAIT)
        
    def refresh(self):
        self.browser.refresh()
        self.browser.implicitly_wait(SERVER_WAIT)
        
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
    
    def __init__(self, app_path, args=None):
        super().__init__()
        self.start_server(app_path, args=args)
        self._comments = []
        self.user1 = self.get_user()
        self.user2 = self.get_user()
        self.user3 = self.get_user()
        self.test_images = [os.path.join("/tmp/test_images", f) 
                            for f in os.listdir("/tmp/test_images") if f.endswith(".jpg")]
        
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
        self.register_user(self.user3)
            
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
    
    def __init__(self, app_path, args=None):
        super().__init__(os.path.join(app_path, "apps/contact_cards"), args=args)
        self.item = ""

    def get_contacts(self):
        return self.browser.find_elements(By.CSS_SELECTOR, "div.contact")

    def step1(self):
        """I can add one item."""
        self.login(self.user1)
        self.goto('index')
        assert len(self.get_contacts()) == 0, "S1-1 There should be no contacts initially."
        self.browser.find_element(By.CSS_SELECTOR, "button#add_button").click()
        time.sleep(SERVER_WAIT)
        cs = self.get_contacts()
        assert len(cs) == 1, "S1-2 A contact has been added."
        assert cs[0].find_element(By.CSS_SELECTOR, "input[name='name']").get_attribute("value") == "", "S1-3 The name is initially not empty."
        assert cs[0].find_element(By.CSS_SELECTOR, "input[name='affiliation']").get_attribute("value") == "", "S1-4 The affiliation is initially not empty."
        assert cs[0].find_element(By.CSS_SELECTOR, "textarea[name='description']").get_attribute("value") == "", "S1-5 The descripton is initially not empty."
        return 1, "Empty item added correctly."
    
    def step2(self):
        c = self.get_contacts()[0]
        title = self.browser.find_element(By.CSS_SELECTOR, "h1.title")
        i_name = c.find_element(By.CSS_SELECTOR, "input[name='name']")
        i_aff = c.find_element(By.CSS_SELECTOR, "input[name='affiliation']")
        assert i_name.get_attribute("readonly"), "S2-1 The name should be readonly initially."
        assert i_aff.get_attribute("readonly"), "S2-2 The affiliation should be readonly initially."
        # Name. 
        i_name.click()
        assert not i_name.get_attribute("readonly"), "S2-3 The name should be editable after clicking."
        self.name = str(uuid.uuid4())
        self.aff = str(uuid.uuid4())
        i_name.send_keys(self.name)
        title.click()
        assert i_name.get_attribute("value") == self.name, "S2-4 The name should be editable."
        assert i_name.get_attribute("readonly"), "S2-5 The name should be readonly after losing focus."
        # Affiliation. 
        i_aff.click()
        assert not i_aff.get_attribute("readonly"), "S2-6 The affiliation should be editable after clicking."
        i_aff.send_keys(self.aff)
        title.click()
        assert i_aff.get_attribute("value") == self.aff, "S2-7 The affiliation should be preserved."
        assert i_aff.get_attribute("readonly"), "S2-8 The affiliation should be readonly after losing focus."
        # Sanity check. 
        assert i_name.get_attribute("value") == self.name, "S2-9 The name should be again preserved."
        return 1, "Name and affiliation change to editable when clicked, and to read-only when they lose focus."

    def step3(self):
        c = self.get_contacts()[0]
        title = self.browser.find_element(By.CSS_SELECTOR, "h1.title")
        i_descr = c.find_element(By.CSS_SELECTOR, "textarea[name='description']")
        assert i_descr.get_attribute("readonly"), "S3-1 The description should be readonly initially."
        i_descr.click()
        assert not i_descr.get_attribute("readonly"), "S3-2 The description should be editable after clicking."
        self.description = str(uuid.uuid4())
        i_descr.send_keys(self.description)
        title.click()
        assert i_descr.get_attribute("value") == self.description, "S3-3 The description could not be edited."
        assert i_descr.get_attribute("readonly"), "S3-4 The description should be readonly after losing focus."
        return 1, "Description changes to editable when clicked, and to read-only when it loses focus."

    def step4(self):
        self.refresh()
        time.sleep(SERVER_WAIT)
        c = self.get_contacts()[0]
        i_name = c.find_element(By.CSS_SELECTOR, "input[name='name']")
        i_aff = c.find_element(By.CSS_SELECTOR, "input[name='affiliation']")
        i_descr = c.find_element(By.CSS_SELECTOR, "textarea[name='description']")
        assert i_name.get_attribute("value") == self.name, "S4-1 The name should be preserved after refresh."
        assert i_aff.get_attribute("value") == self.aff, "S4-2 The affiliation should be preserved after refresh."
        assert i_descr.get_attribute("value") == self.description, "S4-3 The description should be preserved after refresh."
        return 1, "The data is preserved after refresh."
        
    def step5(self):
        title = self.browser.find_element(By.CSS_SELECTOR, "h1.title")
        # We add a new contact. 
        self.browser.find_element(By.CSS_SELECTOR, "button#add_button").click()
        time.sleep(SERVER_WAIT)
        # Then, we check that contact 0 still contains the original contact. 
        c0 = self.get_contacts()[0]
        i_name0 = c0.find_element(By.CSS_SELECTOR, "input[name='name']")
        i_aff0 = c0.find_element(By.CSS_SELECTOR, "input[name='affiliation']")
        i_descr0 = c0.find_element(By.CSS_SELECTOR, "textarea[name='description']")
        assert i_name0.get_attribute("value") == self.name, "S5-1 The name should be preserved after refresh."
        assert i_aff0.get_attribute("value") == self.aff, "S5-2 The affiliation should be preserved after refresh."
        assert i_descr0.get_attribute("value") == self.description, "S5-3 The description should be preserved after refresh."

        # We now select contact 1, which should be the new one. 
        c1 = self.get_contacts()[1]
        i_name1 = c1.find_element(By.CSS_SELECTOR, "input[name='name']")
        i_aff1 = c1.find_element(By.CSS_SELECTOR, "input[name='affiliation']")
        i_descr1 = c1.find_element(By.CSS_SELECTOR, "textarea[name='description']")
        # We create random data for contact 1... 
        self.name1 = str(uuid.uuid4())
        self.aff1 = str(uuid.uuid4())
        # ... and we set it.
        i_name1.click()
        i_name1.send_keys(self.name1)
        i_aff1.click()
        i_aff1.send_keys(self.aff1)
        i_name1.click() # To lose focus.

        # Now we check the persistence. 
        self.refresh()
        time.sleep(SERVER_WAIT)
        c1 = self.get_contacts()[1]
        i_name1 = c1.find_element(By.CSS_SELECTOR, "input[name='name']")
        i_aff1 = c1.find_element(By.CSS_SELECTOR, "input[name='affiliation']")
        i_descr1 = c1.find_element(By.CSS_SELECTOR, "textarea[name='description']")
        c0 = self.get_contacts()[0]
        i_name0 = c0.find_element(By.CSS_SELECTOR, "input[name='name']")
        i_aff0 = c0.find_element(By.CSS_SELECTOR, "input[name='affiliation']")
        i_descr0 = c0.find_element(By.CSS_SELECTOR, "textarea[name='description']")
        assert i_name1.get_attribute("value") == self.name1, "S5-4 Name 1 is ok."
        assert i_aff1.get_attribute("value") == self.aff1, "S5-5 Affiliation 1 is ok ok"
        assert i_name0.get_attribute("value") == self.name, "S5-6 Name 0 is ok."
        assert i_aff0.get_attribute("value") == self.aff, "S5-7 Affiliation 0 is ok."
        return 1, "The data is preserved after refresh even when there are multiple items."        

    def step6(self):
        self.refresh()
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        test_image_urls = []
        # Uploads a figure for each contact. 
        for c in contacts:
            figure = c.find_element(By.CSS_SELECTOR, "figure.photo")
            content = c.find_element(By.CSS_SELECTOR, "div.media-content")
            figure.click()
            inp = self.browser.find_element(By.CSS_SELECTOR, "input[type='file']")
            img = random.choice(self.test_images)
            inp.send_keys(img)
            content.click() # To lose focus and trigger @change. 
            time.sleep(SERVER_WAIT)
            test_image_urls.append(image_to_data_url(img))
        self.refresh()
        time.sleep(SERVER_WAIT)
        # Checks that the images are there. 
        contacts = self.get_contacts()
        for i, c in enumerate(contacts):
            img = c.find_element(By.CSS_SELECTOR, "img.photo")
            img_url = img.get_attribute("src")
            assert img.get_attribute("src") == test_image_urls[i], f"S6-1 The image {i} should be there."
        return 1, "Ihe image can be changed by clicking on the figure tag, and the new image is saved in the database."
    
    def step7(self):
        self.login(self.user2)
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        assert len(contacts) == 0, "S7-1 User 2 should not see any contacts."
        self.login(self.user1)
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        assert len(contacts) == 2, "S7-2 User 1 should see two contacts."
        self.login(self.user2)
        time.sleep(SERVER_WAIT)
         # We add a new contact. 
        self.browser.find_element(By.CSS_SELECTOR, "button#add_button").click()
        c = self.get_contacts()[0]
        i_name = c.find_element(By.CSS_SELECTOR, "input[name='name']")
        i_name.click()
        self.user_2_name = str(uuid.uuid4())
        i_name.send_keys(self.user_2_name)
        container = self.browser.find_element(By.CSS_SELECTOR, "div.container")
        container.click() # To lose focus.
        self.refresh()
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        assert len(contacts) == 1, "S7-3 User 2 should see one contact."
        c = contacts[0]
        i_name = c.find_element(By.CSS_SELECTOR, "input[name='name']")
        assert i_name.get_attribute("value") == self.user_2_name, "S7-4 User 2 should see the new contact."
        return 1, "Users see their own contacts."

    def step8(self):
        self.login(self.user1)
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        assert len(contacts) == 2, "S8-1 User 1 should see two contacts."
        c = contacts[0]
        c.find_element(By.CSS_SELECTOR, "i.delete-button").click()
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        assert len(contacts) == 1, f"S8-2 User 1 should see one contact; it sees {len(contacts)}."
        self.refresh()
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        assert len(contacts) == 1, "S8-3 User 1 should see one contact after refresh."
        self.login(self.user2)
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        assert len(contacts) == 1, "S8-4 User 2 should see one contact."
        self.login(self.user3)
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        assert len(contacts) == 0, "S8-5 User 3 should see no contacts."
        self.login(self.user2)
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        c = contacts[0]
        c.find_element(By.CSS_SELECTOR, "i.delete-button").click()
        time.sleep(0.5)
        contacts = self.get_contacts()
        assert len(contacts) == 0, "S8-6 User 2 should see no contacts."
        self.login(self.user1)
        time.sleep(SERVER_WAIT)
        contacts = self.get_contacts()
        assert len(contacts) == 1, "S8-7 User 1 should see one contact."
        return 1, "Deleting contacts works properly."

if __name__ == "__main__":
    argparser = argparse.ArgumentParser()
    argparser.add_argument("--debug", default=False, action="store_true",
                           help="Run the grading in debug mode.")
    argparser.add_argument("--port", default=8800, type=int, 
                            help="Port to run the server on.")
    args = argparser.parse_args()
    tests = Assignment(".", args=args)
    tests.grade()
