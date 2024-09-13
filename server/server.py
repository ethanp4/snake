from flask import Flask, jsonify, render_template, request
from dotenv import load_dotenv
import os

app = Flask(__name__, template_folder="html")

scores = []
idCount = 0

load_dotenv()

authUser = os.getenv("user")
authPass = os.getenv("pass")

@app.route("/")
def root():
  html = ""
  for score in scores:
    html += f"""
              <tr>
                <td>{score["id"]}</td>
                <td>{score["score"]}</td>
              </tr>
                """
  return render_template("leaderboard.html", scoresList=html)

@app.route("/leaderboard", methods=['GET', 'POST'])
def leaderboard():
  try:
    if authUser != request.authorization.username or authPass != request.authorization.password:
      raise Exception("Incorrect or missing authorization")
  except:
    return "Unauthorized", 401

  if request.method == 'GET':
    return jsonify(scores), 200
  score = request.get_json()["score"]
  if score is None:
    return "Bad request: no score given", 400

  global idCount
  idCount += 1
  id = idCount
  score = {
    "id": id,
    "score": score
  }
  scores.append(score)
  return score, 200

app.run(port=8080)