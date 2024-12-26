import os
import shutil
import subprocess
import time
import traceback
import uuid
import argparse

from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By

from selenium.webdriver.firefox.options import Options as FirefoxOptions

# You can increase this if your server is very slow.
SERVER_WAIT = 0.5

class StopGrading(Exception):
    pass

class py4web(object):

    def start_server(self, path_to_app, args=None):
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
        if not args.debug:
            browser_options.add_argument("--headless")
        self.browser =  webdriver.Chrome(options=browser_options)
        # browser_options = FirefoxOptions()
        # if not args.debug:
        #     browser_options.add_argument("--headless")
        # self.browser = webdriver.Firefox(options=browser_options)

    def __del__(self):
        pass
        if self.server:
            self.stop_server()

    def stop_server(self):
        print("- stopping server...")
        self.server.kill()
        self.server = None
        print("- stopping server...DONE")
        self.browser.quit()

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

    def __init__(self, app_path, args=None):
        super().__init__()
        self.start_server(app_path, args=args)
        self._comments = []
        self.user1 = self.get_user()
        self.user2 = self.get_user()

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

    def __init__(self, app_path, args=None):
        super().__init__(os.path.join(app_path, "apps/shopping"), args=args)
        self.item = ""

    def step1(self):
        """I can add one item."""
        self.login(self.user1)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        self.item = str(uuid.uuid4())
        self.browser.find_element(By.CSS_SELECTOR, "input.add-item").send_keys(self.item)
        self.browser.find_element(By.CSS_SELECTOR, "i.add-item").click()
        time.sleep(SERVER_WAIT)
        item_places = self.browser.find_elements(By.CSS_SELECTOR, "table td.item")
        assert self.item in [i.text for i in item_places], "The item is not added to the list."
        return 1, "Item added correctly."

    def step2(self):
        self.refresh()
        time.sleep(SERVER_WAIT)
        item_places = self.browser.find_elements(By.CSS_SELECTOR, "table td.item")
        assert self.item in [i.text for i in item_places], "The item does not persist."
        return 2, "The item persists."

    def step3(self):
        """Another user cannot see the item."""
        self.login(self.user2)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        item_places = self.browser.find_elements(By.CSS_SELECTOR, "table td.item")
        assert self.item not in [i.text for i in item_places], "The item is visible to another user."
        return 1, "The item is not visible to another user."

    def step4(self):
        self.login(self.user1)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        self.item2 = str(uuid.uuid4())
        self.browser.find_element(By.CSS_SELECTOR, "input.add-item").send_keys(self.item2)
        self.browser.find_element(By.CSS_SELECTOR, "i.add-item").click()
        time.sleep(SERVER_WAIT)
        item_places = self.browser.find_elements(By.CSS_SELECTOR, "table td.item")
        items = [self.item2, self.item]
        for it, pl in zip(items, item_places):
            assert it in pl.text, "The new item should be added to the TOP of the list."
        return 1, "When a second item is added, it's added to the top of the list."

    def step5(self):
        self.refresh()
        time.sleep(SERVER_WAIT)
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        item1 = item_rows[0].find_element(By.CSS_SELECTOR, "td.item").text
        check_box = item_rows[0].find_element(By.CSS_SELECTOR, "td.check input")
        assert not check_box.is_selected(), "The checkbox should not be selected."
        check_box.click()
        time.sleep(SERVER_WAIT)
        # Now the checked item should be in position 2.
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "tr.item-row")
        assert item1 in item_rows[1].find_element(By.CSS_SELECTOR, "td.item").text, "The checked item should be in position 2."
        self.item1 = item1
        self.item0 = item_rows[0].find_element(By.CSS_SELECTOR, "td.item").text
        return 1, "it's possible to mark an item as purchased, and checked items are moved to the bottom of the list."

    def step6(self):
        """Checks persistency of the two items."""
        self.refresh()
        time.sleep(SERVER_WAIT)
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        items = [self.item0, self.item1]
        item_places = [i.text for i in item_rows]
        for ia, ib in zip(items, item_places):
            assert ia == ib, "The items should be in the same order."
        return 1, "When reloading, checked items follow unchecked ones."

    def step7(self):
        self.refresh()
        time.sleep(SERVER_WAIT)
        self.item = str(uuid.uuid4())
        self.browser.find_element(By.CSS_SELECTOR, "input.add-item").send_keys(self.item)
        self.browser.find_element(By.CSS_SELECTOR, "i.add-item").click()
        time.sleep(SERVER_WAIT)
        item_places = self.browser.find_elements(By.CSS_SELECTOR, "table td.item")
        assert self.item in item_places[0].text, "The new item should be added to the top of the list."
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        first_check_box = item_rows[0].find_element(By.CSS_SELECTOR, "td.check input")
        last_check_box = item_rows[-1].find_element(By.CSS_SELECTOR, "td.check input")
        assert not first_check_box.is_selected(), "The first checkbox should not be selected."
        assert last_check_box.is_selected(), "The last checkbox should be selected."
        # If I click on the first item, it should become the last.
        first_check_box.click()
        time.sleep(SERVER_WAIT)
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        assert self.item in item_rows[-1].text, "The new item should be moved to the last position."
        # Now if I unclick the middle row,
        middle_check_box = item_rows[1].find_element(By.CSS_SELECTOR, "td.check input")
        middle_item = item_rows[1].find_element(By.CSS_SELECTOR, "td.item").text
        middle_check_box.click()
        time.sleep(SERVER_WAIT)
        # The middle row should be moved to the top.
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        assert middle_item in item_rows[0].text, "The middle item should be moved to the top."
        self.refresh()
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        checkboxes = [i.find_element(By.CSS_SELECTOR, "td.check input").is_selected() for i in item_rows]
        assert checkboxes == [False, False, True], "The checkboxes should be in the correct state."
        return 1, "The dynamics of checking/unchecking items are correct."

    def step8(self):
        self.login(self.user2)
        self.goto('index')
        time.sleep(SERVER_WAIT)
        self.item0 = str(uuid.uuid4())
        self.item1 = str(uuid.uuid4())
        self.browser.find_element(By.CSS_SELECTOR, "input.add-item").send_keys(self.item0)
        self.browser.find_element(By.CSS_SELECTOR, "i.add-item").click()
        time.sleep(SERVER_WAIT)
        self.browser.find_element(By.CSS_SELECTOR, "input.add-item").send_keys(self.item1)
        self.browser.find_element(By.CSS_SELECTOR, "i.add-item").click()
        time.sleep(SERVER_WAIT)
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        items = [i.find_element(By.CSS_SELECTOR, "td.item").text for i in item_rows]
        assert [self.item1, self.item0] == items, "The last inserted item should be first."
        delete_buttons = [i.find_element(By.CSS_SELECTOR, "td.trash i") for i in item_rows]
        assert len(delete_buttons) == 2, "There should be two delete buttons."
        delete_buttons[0].click()
        time.sleep(SERVER_WAIT)
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        assert len(item_rows) == 1, "There should be one item left."
        assert self.item0 in item_rows[0].find_element(By.CSS_SELECTOR, "td.item").text, "The correct item should be left."
        self.refresh()
        time.sleep(SERVER_WAIT)
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        assert len(item_rows) == 1, "There should be one item left after refresh."
        assert self.item0 in item_rows[0].find_element(By.CSS_SELECTOR, "td.item").text, "The correct item should be left after refresh."
        return 1, "Deletion works correctly."

    def step9(self):
        self.login(self.user1)
        self.goto('index')
        self.browser.implicitly_wait(0.2)
        item_rows = self.browser.find_elements(By.CSS_SELECTOR, "table tr.item-row")
        assert len(item_rows) == 3, "The items of the other user should be visible."
        return 1, "No interference between users."

    # I would like to check that a user cannot delete another user's items, but don't know
    # how to check that in selenium.

if __name__ == "__main__":
    argparser = argparse.ArgumentParser()
    argparser.add_argument("--debug", default=False, action="store_true",
                           help="Run the grading in debug mode.")
    argparser.add_argument("--port", default=8800, type=int, 
                            help="Port to run the server on.")
    args = argparser.parse_args()
    tests = Assignment(".", args=args)
    tests.grade()
