# contact_us: A contacts-us website

In this assignment, you will create a contact form for a website. 
The goal is to build the site using entirely server-side code, without any JavaScript.
You should use py4ewb forms and grids to implement the site. 
 
The website has two pages: 

- `index`, which displays the contact form for visitors. 
- `contact_requests`, which displays the contact requests for site admins. 

To implement this, you will need to modify the files:

- `models.py`, to create the database tables (). In the instructor solution, this file is 24 lines long.
- `controllers.py`, to implement the logic for the two pages. In the instructor solution, this file is 34 lines long.
- `index.html` and `contact_requests.html`, to implement the two pages. In the instructor solution, these files are both 8 lines long (and they could be shorter!).

So the total amount of code you need to write is quite small.  
This assignment illustrates the power of py4web in creating small, but powerful, web applications.

## `index` page

The `index` page will have a form with the following fields:

- name: a text field for the name of the visitor. You should check that the user does not leave this field empty (hint: see the [validators](https://py4web.com/_documentation/static/en/chapter-12.html#text-format-validators) in py4web).
- email: a text field for the email of the visitor. You should check that the email has the correct format (hint: see the [validators](https://py4web.com/_documentation/static/en/chapter-12.html#text-format-validators) in py4web).
- phone: a text field for the phone of the visitor. Again, you should check that the field is non-empty.
- message: a text area for the message of the visitor.

When a form is submitted correctly, it should redirect to the `index` page again, so that one can enter more contact information. 

## `contact_requests` page

The `contact_requests` page will display the contact requests in reverse chronological order.
The page should **only be accessible to the `admin@example.com` user**.
Non-logged in users, or users with other emails, should be redirected to the `index` page.

The page should display a [grid](https://py4web.com/_documentation/static/en/chapter-14.html), displaying the contact requests.
The grid should be searchable via: 

- By name: a text field that filters the requests by the name of the requestor.  If one types `uc`, then `Luca`, `Lucas`, and `Lucia` should all be part of the result, but `Alice` should not.
- By message: a text field that filters the requests that contain the given string in the message.  If one types `ogin` then clicks on the search button, a request with the message `I cannot find the login button` should be displayed, but one with the message `I am an ogre` should not.

## Database

You need to create a table, called `contact_requests`, with fields with the following names: 

- `name`
- `email`
- `phone`
- `message`

Using different names may cause the grading to fail. 

## Grading

You can grade the assignment yourself, like this: 

    python grade.py

The grading is as follows.  8 points are assigned automatically via the grading script: 

- 1 point: the form to create a contact request contains the required fields, and we can submit the form.
- 1 point: the form to create a contact request validates that the name and phone are non-empty.
- 1 point: the form to createa a contact request validates that the email has the correct format.
- 1 point: the `contact_requests` page is only accessible to the `admin@example.com` user; others are redirected to the `index` page.
- 1 point: the `contact_requests` page displays the contact requests.
- 1 point: the contact requests in the grid are searchable by name.
- 1 point: the contact requests in the grid are searchable by message.
- 1 point: each contact request has a delete button, displayed by the grid, which deletes the request when pressed. 

The remaining 2 points are assigned manually, and are based on: 

- 1 point: forms and grid from py4web are used. 
- 1 point: the code is concise (no more than 2x the length of the instructor solution).

## Killing old servers

Sometimes, you may need to kill old servers.  You can list the old servers via: 

    ps aux | grep 'py4web'

If you have more than one server associated with port 8800, a new server 
will not be created, and this is a common source of errors.  You can kill 
all leftover py4web servers via:

    pkill -f 'py4web'

## Submission

To submit, first crete the zip file, via: 

    python zipit.py

This creates the file `submission.zip`.  Submit this file to the Google Form, and **be sure to press SUBMIT on the form**.  Just uploading the file to the form is not enough. 
