from flask import Flask, jsonify, render_template, request

app = Flask(__name__, template_folder="html", static_folder="static")

scores = []
idCount = 0

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

@app.route("/leaderboard", methods=['GET'])
def getLeaderboard():
  return jsonify(scores), 200

@app.route("/leaderboard", methods=['POST'])
def postScore():
  global idCount
  score = request.get_json()["score"]
  
  if score is None:
    return "Bad request: no score given", 400

  idCount += 1
  id = idCount
  score = {
    "id": id,
    "score": score
  }
  scores.append(score)
  return score, 200

app.run(port=8080)