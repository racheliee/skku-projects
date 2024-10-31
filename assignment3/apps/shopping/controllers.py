"""
This file defines actions, i.e. functions the URLs are mapped into
The @action(path) decorator exposed the function at URL:

    http://127.0.0.1:8000/{app_name}/{path}

If app_name == '_default' then simply

    http://127.0.0.1:8000/{path}

If path == 'index' it can be omitted:

    http://127.0.0.1:8000/

The path follows the bottlepy syntax.

@action.uses('generic.html')  indicates that the action uses the generic.html template
@action.uses(session)         indicates that the action uses the session
@action.uses(db)              indicates that the action uses the db
@action.uses(T)               indicates that the action uses the i18n & pluralization
@action.uses(auth.user)       indicates that the action requires a logged in user
@action.uses(auth)            indicates that the action requires the auth object

session, db, T, auth, and tempates are examples of Fixtures.
Warning: Fixtures MUST be declared with @action.uses({fixtures}) else your app will result in undefined behavior
"""

from py4web import action, request, abort, redirect, URL
from yatl.helpers import A
from .common import db, session, T, cache, auth, logger, authenticated, unauthenticated, flash
from py4web.utils.url_signer import URLSigner
from .models import get_user_email
import datetime

url_signer = URLSigner(session)

@action('index')
@action.uses('index.html', db, auth.user)
def index():
    return dict(
        # For example...
        load_data_url = URL('load_data'),
        # Add other things here.
        add_item_url = URL('add_item'),
        toggle_item_url = URL('toggle_item'),
        delete_item_url = URL('delete_item'),
    )

@action('load_data')
@action.uses(db, auth.user)
def load_data():
    # Complete.
    user_id = auth.current_user.get('id')
    items = db(db.shopping_item.user_id == user_id).select(orderby=(db.shopping_item.purchased, ~db.shopping_item.last_modified)).as_list()
    return dict(items=items)
    # return dict()

# You can add other controllers here.
@action('add_item', method='POST')
@action.uses(db, auth.user)
def add_item():
    user_id = auth.current_user.get('id')
    item_name = request.json.get('item_name')
    if item_name:
        db.shopping_item.insert(item_name=item_name, user_id=user_id, last_modified=datetime.datetime.utcnow())
    return "ok"

@action('toggle_item', method='POST')
@action.uses(db, auth.user)
def toggle_item():
    item_id = request.json.get('item_id')
    item = db.shopping_item[item_id]
    if item and item.user_id == auth.current_user.get('id'):
        item.update_record(purchased=not item.purchased, last_modified=datetime.datetime.utcnow())
    # else:
    #     item.update_record(purchased=not item.purchased)
    return "ok"

@action('delete_item', method='POST')
@action.uses(db, auth.user)
def delete_item():
    item_id = request.json.get('item_id')
    item = db.shopping_item[item_id]
    if item and item.user_id == auth.current_user.get('id'):
        db(db.shopping_item.id == item_id).delete()
    return "ok"