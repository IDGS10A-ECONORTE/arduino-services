from flask import Flask
from ingest import subroute
from dotenv import load_dotenv

# Crear la aplicación principal.
app = Flask(__name__)

#Blueprints
app.register_blueprint(subroute, url_prefix='/api')

# Ruta principal
@app.route("/")
def home():
    return "Página raiz de flask"

# Punto de entrada
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=7777, debug=True)
