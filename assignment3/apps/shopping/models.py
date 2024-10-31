"""
This file defines the database models
"""

import datetime
from .common import db, Field, auth
from pydal.validators import *


def get_user_email():
    return auth.current_user.get('email') if auth.current_user else None

def get_time():
    return datetime.datetime.utcnow()

# Add here any table definition you need. Below is an example.
# db.define_table('shopping_list',
#    Field('product_name', requires=IS_NOT_EMPTY()),
#    )

db.define_table(
    'shopping_item',
    Field('item_name', requires=IS_NOT_EMPTY()),
    Field('purchased', 'boolean', default=False),
    Field('user_id', 'reference auth_user', default=auth.current_user.get('id') if auth.current_user else None, readable=False, writable=False),
    Field('last_modified', 'datetime', default=get_time, readable=False, writable=False),
)

db.commit()
