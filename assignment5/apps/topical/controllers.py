from py4web import action, request, URL
from .common import auth
from .models import db, parse_post_content, get_user_email

# Complete. 
@action('index')
@action.uses('index.html', db)
def index():
    return dict(
        get_posts_url=URL('get_posts'),
        create_post_url=URL('create_post'),
        delete_post_url=URL('delete_post'),
        get_tags_url=URL('get_tags'),
    )

@action('get_posts')
@action.uses(db)
def get_posts():
    posts = db(db.post).select(orderby=~db.post.created_at).as_list()
    return {"posts": posts}

@action('create_post', method="POST")
@action.uses(db)
def create_post():
    post_content = request.json.get("content", "")
    print(f"Received post content: {post_content}")  # Debug log
    if not post_content:
        print("Error: Post content is empty.")
        return {"error": "Post content cannot be empty"}
    tags = parse_post_content(post_content)
    try:
        post_id = db.post.insert(content=post_content, tags=tags)
        print(f"Post created with ID: {post_id}, tags: {tags}")  # Debug log
        return {"post_id": post_id, "tags": tags}
    except Exception as e:
        print(f"Error while creating post: {e}")
        return {"error": "An error occurred while creating the post"}


@action('delete_post/<post_id>', method="DELETE")
@action.uses(db)
def delete_post(post_id):
    db(db.post.id == post_id).delete()
    return {"success": True}

@action('get_tags')
@action.uses(db)
def get_tags():
    tags = db(db.post).select(db.post.tags, distinct=True)
    tag_list = set(tag for row in tags for tag in row.tags.split(",") if tag)
    return {"tags": [tag.replace("#", "") for tag in tag_list]}
