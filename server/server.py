from flask import Flask, jsonify, render_template, request

app = Flask(__name__, template_folder="html", static_folder="static")

scores = []
idCount = 0

@app.route("/")
def root():
  return render_template("leaderboard.html", scoresList=scores)

@app.route("/leaderboard", methods=['GET'])
def getLeaderboard():
  return jsonify(scores), 200

@app.route("/leaderboard", methods=['POST'])
def postScore():
  global idCount
  score = request.args.get('score')
  # print(request.args.getlist)
  print(request.get_json())
  
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