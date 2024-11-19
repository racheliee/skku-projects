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
        self.debug = args.debug
        print("Starting the server")
        self.port = args.port
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
        browser_options = webdriver.ChromeOptions()
        if not self.debug:
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
        self.browser.get(f"http://127.0.0.1:{args.port}/{self.app_name}/{path}")
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
    
    def __init__(self, app_path, args=None):
        super().__init__()
        self.start_server(app_path, args=args)
        self._comments = []
        self.user1 = self.get_user()
        self.user2 = self.get_user()
        self.user3 = self.get_user()
        
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
        super().__init__(os.path.join(app_path, "apps/topical"), args=args)
        self.item = ""
        self.tag1 = "".join(random.choices("abcdefghilmnopqrstuvz", k=8))
        self.tag2 = "".join(random.choices("abcdefghilmnopqrstuvz", k=8))
        self.tag3 = "".join(random.choices("abcdefghilmnopqrstuvz", k=8))

    def get_posts(self):
        return self.browser.find_elements(By.CSS_SELECTOR, "div.post")

    def get_tags(self):
        return self.browser.find_elements(By.CSS_SELECTOR, "button.tag")
    
    def find_tag(self, tag_name):
        tags = self.get_tags()
        for tag in tags:
            if tag.text == tag_name:
                return tag
        return None

    def step1(self):
        """Adding posts"""
        self.login(self.user1)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 0, "S1-1 There should be no posts initially."
        assert len(self.get_tags()) == 0, "S1-2 There should be no tags initially."
        self.browser.find_element(By.CSS_SELECTOR, "textarea#post-input").send_keys(
            f"I love #pasta with #anchovies and #{self.tag1}.")
        self.browser.find_element(By.CSS_SELECTOR, "#post-button").click()
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 1, "S1-3 There should be one post."
        self.login(self.user2)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 1, "S1-4 The second user should also see the post."
        self.browser.find_element(By.CSS_SELECTOR, "textarea#post-input").send_keys(
            f"I would never eat #{self.tag1} and #{self.tag2} together.")
        self.browser.find_element(By.CSS_SELECTOR, "#post-button").click()
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 2, "S1-5 Now there should be two posts."
        self.login(self.user1)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 2, "S1-6 There should still be two posts."
        return 1, "Posts are added correctly."

    def step2(self):
        "Posts in chronological order"
        self.refresh()
        time.sleep(SERVER_WAIT)
        self.browser.find_element(By.CSS_SELECTOR, "textarea#post-input").send_keys(
            f"I like #pasta with #{self.tag3}.")
        self.browser.find_element(By.CSS_SELECTOR, "#post-button").click()
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 3, "S2-1 Now there should be two posts."
        posts = self.get_posts()
        assert posts[0].find_element(By.CSS_SELECTOR, "p.post-content").text == f"I like #pasta with #{self.tag3}.", "S2-2 The newest post should be correct."
        assert posts[2].find_element(By.CSS_SELECTOR, "p.post-content").text == f"I love #pasta with #anchovies and #{self.tag1}.", "S2-3 The oldest post should be correct."
        return 1, "Posts are in chronological order."
        
    def step3(self):
        """Adding tags"""
        self.login(self.user1)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        assert len(self.get_tags()) == 5, "S3-1 There should be four tags."
        tags = self.get_tags()
        tag_text = set([tag.text for tag in tags])
        assert tag_text == {"pasta", "anchovies", self.tag1, self.tag2, self.tag3}, f"S3-2 The tags should be #pasta, #anchovies, #{self.tag1}, #{self.tag2}."
        return 1, "Tags work correctly."
    
    def step4(self):
        """Filtering posts"""
        pasta_tag = self.find_tag("pasta")
        pasta_tag.click()
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 2, "S4-1 There should be two posts with pasta."
        pasta_tag.click()
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 3, "S4-2 There should be three posts."
        anchovies_tag = self.find_tag(self.tag3)
        anchovies_tag.click()
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 1, f"S4-3 There should be one post with #{self.tag3}."
        self.login(self.user2)
        time.sleep(SERVER_WAIT)
        anchovies_tag = self.find_tag(self.tag3)
        anchovies_tag.click()
        time.sleep(SERVER_WAIT)
        assert len(self.get_posts()) == 1, f"S4-4 There should be one post with #{self.tag3} also for user 2."
        return 1, "Filtering works correctly."
    
    def step5(self):
        """Deletion."""
        self.login(self.user2)
        time.sleep(SERVER_WAIT)
        posts = self.get_posts()
        has_button = [len(p.find_elements(By.CSS_SELECTOR, "button.delete-button")) > 0 for p in posts]
        assert has_button == [False, True, False], "S5-1 User 2 should be able to edit only the middle post."
        self.login(self.user1)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        posts = self.get_posts()
        has_button = [len(p.find_elements(By.CSS_SELECTOR, "button.delete-button")) > 0 for p in posts]
        assert has_button == [True, False, True], "S5-2 User 1 should be able to edit only the middle post."
        button2 = posts[2].find_element(By.CSS_SELECTOR, "button.delete-button")
        button2.click()
        time.sleep(SERVER_WAIT)
        posts = self.get_posts()
        assert len(posts) == 2, "S5-3 There should be two posts after one is deleted."
        return 1, "The deletion button works correctly."

    def step6(self):
        """Tags and deletion."""        
        tag_texts = [t.text for t in self.get_tags()]
        assert set(tag_texts) == {"pasta", self.tag1, self.tag2, self.tag3}, f"S6-1 After deleting the oldest post, the tags should be #pasta, #{self.tag1}, #{self.tag2}, #{self.tag3}."
        self.login(self.user2)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        posts = self.get_posts()
        button1 = posts[1].find_element(By.CSS_SELECTOR, "button.delete-button")
        button1.click()
        time.sleep(SERVER_WAIT)
        tag_texts = [t.text for t in self.get_tags()]
        assert set(tag_texts) == {"pasta", self.tag3}, f"S6-2 After deleting the middle post, the tags should be #pasta, #{self.tag3}."
        self.login(self.user1)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        posts = self.get_posts()
        button0 = posts[0].find_element(By.CSS_SELECTOR, "button.delete-button")
        button0.click()
        time.sleep(SERVER_WAIT)
        tag_texts = [t.text for t in self.get_tags()]
        assert len(tag_texts) == 0, "S6-3 After deleting the last post, there should be no tags."
        return 1, "Tags and deletion work correctly."
        
            
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--debug', default=False, action='store_true',
                        help="Debug mode (show browser).")
    parser.add_argument("--port", default=8800, type=int, 
                            help="Port to run the server on.")
    args = parser.parse_args()
    tests = Assignment(".", args=args)
    tests.grade()
