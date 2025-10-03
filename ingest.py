from flask import Blueprint

subroute = Blueprint('ingest', __name__)

@subroute.route('/')
def ingest():
    return "Estás en el módulo de ingesta"

@subroute.route('/data')
def data():
    return "Informacion Ingestada"