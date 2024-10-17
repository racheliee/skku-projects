import os
import sys
import numpy as np
import random

from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By

DEBUG = False
browser_options = webdriver.ChromeOptions()
if not DEBUG:
    browser_options.add_argument("--headless")

class StopGrading(Exception):
    pass

class Assignment(object):
    
    def __init__(self, path):
        self._comments = []
        fullpath = os.path.abspath(path)
        url = 'file://' + os.path.join(fullpath, 'assignment/index.html')
        print(f"Grading {url}")
        self.browser =  webdriver.Chrome(options=browser_options)
        self.goto(url)
        
    def goto(self, url):
        self.browser.get(url)
        self.browser.implicitly_wait(0.2)
        
    def refresh(self):
        self.browser.refresh()
        self.browser.implicitly_wait(0.2)
       
    def append_comment(self, points, comment):
        self._comments.append((points, comment))

    def get_board(self):
        table = self.browser.find_element(By.CSS_SELECTOR, 'table')
        cells = table.find_elements(By.CSS_SELECTOR, 'td')
        assert len(cells) == 9, "Wrong number of cells in the table"
        cell_content = np.zeros((3, 3), dtype=int)
        cell_won = np.zeros((3, 3), dtype=int)
        buttons = [[None, None, None], [None, None, None], [None, None, None]]
        for i, cell in enumerate(cells):
            row = i // 3
            col = i % 3
            buttons[row][col] = cell.find_element(By.CSS_SELECTOR, 'button')
            b = cell.find_element(By.CSS_SELECTOR, 'button')
            if "is-success" in b.get_attribute('class'):
                cell_won[row, col] = 1
            s = cell.find_element(By.CSS_SELECTOR, 'span i')
            if "fa-remove" in s.get_attribute('class'):
                cell_content[row, col] = 1
            elif "fa-circle-o" in s.get_attribute('class'):
               cell_content[row, col] = 2
        return cell_content, cell_won, buttons
        
    def clear_game(self):
        new_game = self.browser.find_element(By.CSS_SELECTOR, 'button#new-game')
        new_game.click()
        cell_content, cell_won, buttons = self.get_board()
        assert np.all(cell_content == 0), "After clicking on new game, board is not empty"
        assert np.all(cell_won == 0), "After clicking on new game, someone has won"
        
    def check_won(self, cell_content):
        won = np.zeros((3, 3), dtype=int)
        for p in [1, 2]:
            for i in range(3):
                if np.all(cell_content[i, :] == p):
                    won[i, :] = 1
                if np.all(cell_content[:, i] == p):
                    won[:, i] = 1
            if np.all(cell_content.diagonal() == p):
                for i in range(3):
                    won[i, i] = 1
            if np.all(cell_content[::-1].diagonal() == p):
                for i in range(3):
                    won[i, 2 - i] = 1
        return won

    def step01(self):
        assert self.browser.find_element(By.TAG_NAME, 'html')
        cell_content, cell_won, buttons = self.get_board()
        return 1, "HTML is valid and board is there"
    
    def step02(self):
        cell_content, cell_won, buttons = self.get_board()
        assert np.all(cell_content == 0), "Initial board is not empty"
        assert np.all(cell_won == 0), "Nobody has won yet"
        return 1, "Initial board is empty"
    
    def step03(self):
        self.clear_game()
        return 1, "The new game button works"

    def step04(self):
        for i in range(3):
            for j in range(3):
                self.clear_game()
                cell_content, cell_won, buttons = self.get_board()
                buttons[i][j].click()
                cell_content, cell_won, buttons = self.get_board()
                assert cell_content[i, j] == 1, f"Cell {i}, {j} is not X: {cell_content[i, j]}"        
        return 1, "First move is X"
    
    def step05(self):
        for _ in range(20):
            self.clear_game()
            true_board = np.zeros((3, 3), dtype=int)
            player = 1
            for _ in range(9):
                cell_content, cell_won, buttons = self.get_board()
                if np.sum(self.check_won(true_board)) > 0:
                    break
                i, j = random.randint(0, 2), random.randint(0, 2)
                buttons[i][j].click()
                if cell_content[i, j] == 0:
                    true_board[i, j] = player
                    player = 3 - player
                cell_content, cell_won, buttons = self.get_board()
                assert np.all(cell_content == true_board), f"Board is not as expected:\n{cell_content}\n{true_board}"
        return 3, "The game playing works."

    def step06(self):
        for _ in range(40):
            self.clear_game()
            true_board = np.zeros((3, 3), dtype=int)
            player = 1
            for _ in range(9):
                cell_content, cell_won, buttons = self.get_board()
                if np.sum(self.check_won(true_board)) > 0:
                    break                       
                i, j = random.randint(0, 2), random.randint(0, 2)
                buttons[i][j].click()
                if cell_content[i, j] == 0 and np.all(self.check_won(true_board) == 0):
                    true_board[i, j] = player
                    player = 3 - player
                cell_content, cell_won, buttons = self.get_board()
                assert np.all(cell_content == true_board), f"Board is not as expected:\n{cell_content}\n{true_board}"
                assert np.all(cell_won == self.check_won(true_board)), f"Cell won is not as expected:\n{cell_won}\n{self.check_won(true_board)}"
        return 3, "The game winning works."
    
    def grade(self):
        steps = [getattr(self, name) for name in dir(self) if name.startswith("step")]
        for step in steps:
            try:
                g, c = step()
                self.append_comment(g, step.__name__ + f": {g} point(s): {c}")
            except StopGrading:
                break
            except Exception as e:
                self.append_comment(0, f"Error in {step.__name__}: {e}")
        grade = 0
        for points, comment in self._comments:
            print("=" * 40)
            print(f"[{points} points]", comment)
            grade += points
        print("=" * 40)
        print(f"TOTAL GRADE {grade}")
        print("=" * 40)
        self.browser.quit()
        return grade

if __name__ == "__main__":
    tests = Assignment(".")
    tests.grade()
