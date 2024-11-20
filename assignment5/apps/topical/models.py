"""
This file defines the database models
"""
import datetime
import re

from .common import db, Field, auth
from pydal.validators import *

def get_user_email():
    return auth.current_user.get('email') if auth.current_user else None

def parse_post_content(content):
    tags = re.findall(r"#(\w+)", content)
    return ",".join(tags)

def get_user():
    return auth.current_user.get('id') if auth.current_user else None

def get_time():
    return datetime.datetime.utcnow()

# Complete. 

db.define_table(
    "post",
    Field("content", "text", requires=IS_NOT_EMPTY()),
    Field("tags", "text"),
    Field("user_email", default=get_user_email),
    Field("created_at", "datetime", default=datetime.datetime.utcnow),
)