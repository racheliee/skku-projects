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


db.define_table(
    'contact_card',
    Field('user_email', default=get_user_email),
    Field('contact_name'),
    Field('contact_affiliation'),
    Field('contact_description', 'text'),
    Field('contact_image', 'text'),
    Field('photo_url', 'text', default='https://bulma.io/assets/images/placeholders/96x96.png'),
    Field('created_on', 'datetime', default=get_time),
)

db.commit()
