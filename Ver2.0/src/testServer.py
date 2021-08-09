# import main Flask class and request object
from flask import Flask, request

# create the Flask app
app = Flask(__name__)

@app.route('/logData/', methods=['POST'])
def parse_request():
    app.logger.info(request.json);
    return ("OK", 200);

if __name__ == '__main__':
    # run app in debug mode on port 5000
    app.run(debug=True, port=3000, host="0.0.0.0")