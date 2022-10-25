const sql = require("./db.js");

// constructor
const Capteur = function(capteur) {
    this.id_user = capteur.id_user;
    this.longitude = capteur.longitude;
    this.latitude = capteur.latitude;
    this.type = capteur.type;
  };

 

Capteur.create = (newCapteur, result) => {
    sql.query("INSERT INTO alert SET ?", newCapteur, (err, res) => {
      if (err) {
        console.log("error: ", err);
        result(err, null);
        return;
      }
  
      console.log("created position: ", { id: res.insertId, ...newCapteur });
      result(null, { id: res.insertId, ...newCapteur });
    });
  };

Capteur.getAllPublished = result => {
    sql.query("SELECT * FROM position", (err, res) => {
      if (err) {
        console.log("error: ", err);
        result(null, err);
        return;
      }
  
      console.log("capteurs: ", res);
      result(null, res);
    });
  };

  Capteur.getRedZone = result => {
    sql.query("SELECT * FROM red_zone", (err, res) => {
      if (err) {
        console.log("error: ", err);
        result(null, err);
        return;
      }
  
      console.log("red_zone: ", res);
      result(null, res);
    });
  };
  
  module.exports = Capteur;
  