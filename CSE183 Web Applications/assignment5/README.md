# Topical App

Write a Topical app using Vue.js and the provided starter code.
The app allows users to create posts in a common feed (all posts, by all users, end up in the same feed). 

## Behavior 

Please refer to the [video](demo.mp4) for a visual representation of the app.

### Screen organization

The screen should be divided into two columns, one 3/4 wide, and one 1/4 wide. 
In the left column, the 3/4 wide, there is an area for creating posts, and a list of posts.  In the right column, the 1/4 wide, there is a list of tags. 

### Post creation

To create a post, a user enters text in a `textarea#post-input` (a textarea element with id `post-input`), and clicks a button with id `post-button`.  Only logged in users should be able to create posts. 

### Post display

A post and all related information should be displayed in a `div.post` element.
Within it, the posts text should be displayed in a `p.post-content` element. 
The posts should be displayed in reverse chronological order (newest on top). 

### Delete button

If a post has been created by the user who is currently logged in, the post should have a delete button with class `delete-button` that allows the user to delete the post. The button should be within the `div.post` element that encloses the post.
If the post was created by a user _other_ than the one currently logged in, the delete button should not be displayed, and the post should not be deletable. 

### Tags

The app should support tags.  When a post is created, it is parsed.  A tag is a word with an initial `#` character.  For example, in the post `Hello #world`, the tag is `#world`.  A post can have multiple tags. 

The tags should be displayed in the right column, in a `div.tags` element.  Each tag should be displayed in a `button.tag` element, and importantly, for a tag `#world`, the string `world` should be displayed, without the leading `#` (this choice is arbitrary, but having a uniform choice makes testing easier). 
In the example above, there should be an element `button.tag` containing `world` as text.  
Again refere to the video for examples. 

Initially, all tags are inactive.  When a tag its clicked, its active/inactive state is toggled. 
The display logic for posts is as follows: 

- If all tags are inactive, all posts are displayed.
- If some tags are active, only posts that have at least one of the active tags are displayed.

### Tags and post deletion

The left bar should display all, and only, the tags that are present in some post.  So if a post $p$ contains the tag #a, and no other post contains #a, the tag #a should be displayed in the right column. But if the post $p$ is deleted, the tag #a should disappear from the right column.


## Implementation

To run py4web, you can do: 

    ./py4web.sh

The app will be available at http://localhost:8000/topical.
Of course, do look into that script.  All it does is to run the command: 

    py4web run --errorlog=:stdout -L 20 apps

where the options are to optimize logging. 

You should only modify the following files:
- `static/js/index.js`
- `templates/index.html`
- `controllers.py`
- `models.py`

## Grading

You can grade the assignment yourself, like this: 

    python grade.py

The grading is as follows.  6 points are assigned automatically via the grading script: 

- 1 point: post creation. The user can create posts. 
- 1 point: posts are displayed in reverse chronological order.
- 1 point: tags. The user can create tags by including them in the post text, and the tags are displayed in the right column.
- 1 point: tag filtering. The user can filter posts by clicking on tags, toggling their state.
- 1 point: post deletion. The user can delete posts that they created.
- 1 point: tag deletion. When a post is deleted, the tags that are no longer present in any post are removed from the right column.

The remaining 4 points are assigned manually, and are based on: 

- 1 point: code quality. The app is written in vue.js, and the code is clean and well-organized.
- 1 point: security.  Only the user who created a post can delete it. 
- 1 point: esthetics. The app looks decent. 
- 1 point: database organization. The database tables and logic are organized so that the actions of different users do not interfere with each other, and that all operations are efficient (you do not need to create indices, but all operations should be supported via simple, efficient queries). 

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
