import sys
import argparse
import json
import psycopg2
from psycopg2 import sql

# http://initd.org/psycopg/docs/sql.html#module-psycopg2.sql
def store_dataset(db, cur, values_dict):
    attributes = values_dict.keys()
    query = sql.SQL("INSERT INTO dataset ({}) VALUES ({})").format(
        sql.SQL(', ').join(map(sql.Identifier, attributes)),
        sql.SQL(', ').join(map(sql.Placeholder, attributes))
    )
    print(query.as_string(cur))
    cur.execute(query, values_dict)
    db.commit()

# parser = argparse.ArgumentParser()
# parser.add_argument(
#     "--datasets",
#     nargs='+',
#     type=str,
#     help="Path to dataset."
# )
# parser.add_argument(
#     '--connection-string',
#     type=str,
#     dest='conn_string',
#     action='store',
#     help="Specify a custom database connection string (default: empty string)",
#     default="")
#
# args = parser.parse_args()

# Connect to database.
db = psycopg2.connect("")
# Open a cursor to perform database operations
cur = db.cursor()

example_dataset_values_dict = {
    'short_name' : 'D1',
    'filename' : 'dataset_1.bracket',
    'short_description' : 'First example dataset.',
    'number_trees' : 100,
    'avg_tree_size' : 10,
    'min_tree_size' : 10,
    'max_tree_size' : 10
}

store_dataset(db, cur, example_dataset_values_dict)

# Close the cursor.
cur.close()
# Close communication with the database.
db.close()
