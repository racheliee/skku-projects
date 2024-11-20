from py4web import action, request, URL
from .common import auth
from .models import db, parse_post_content, get_user_email
from datetime import datetime

# Complete. 
@action('index')
@action.uses('index.html', db, auth.user)   
def index():
    user_email = auth.get_user()["email"] if auth.get_user() else None
    return dict(
        get_posts_url=URL('get_posts'),
        create_post_url=URL('create_post'),
        delete_post_url=URL('delete_post'),
        get_tags_url=URL('get_tags'),
        current_user_email=user_email
    )

@action('get_posts')
@action.uses(db)
def get_posts():
    posts = db(db.post).select(orderby=~db.post.created_at).as_list()
    for post in posts:
        post['tags'] = post['tags'].split(",") if post['tags'] else []
    print(f"Posts: {posts}")
    return {"posts": posts}

@action('create_post', method="POST")
@action.uses(db, auth.user)
def create_post():
    user_email = auth.get_user()["email"]
    post_content = request.json.get("content", "")
    if not post_content:
        return {"error": "Post content cannot be empty"}
    tags = parse_post_content(post_content)
    post_id = db.post.insert(
        content=post_content, tags=tags, created_at=datetime.utcnow(), user_email=user_email
    )
    return {"post_id": post_id, "tags": tags}


@action('delete_post/<post_id>', method="DELETE")
@action.uses(db, auth.user)
def delete_post(post_id):
    db(db.post.id == post_id).delete()
    return {"success": True}

@action('get_tags')
@action.uses(db, auth.user)
def get_tags():
    tags = db(db.post).select(db.post.tags, distinct=True)
    tag_list = set(tag for row in tags for tag in row.tags.split(",") if tag)
    return {"tags": [tag.replace("#", "") for tag in tag_list]}
