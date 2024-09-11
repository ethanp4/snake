from flask import Flask, request

app = Flask(__name__)

@app.route("/")
def root():
  return "<p>Hello</p>"

@app.route("/leaderboard", methods=['GET', 'POST'])
def leaderboard():
  match request.method:
    case "POST":
      print("posting")
    case "GET":
      print("getting")

app.run(port=8080)