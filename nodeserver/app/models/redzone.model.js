const sql = require("./db.js");

const RedZone = function(red_zone) {
    this.long1 = red_zone.long1;
    this.lat1 = red_zone.lat1;
    this.long2 = red_zone.long2;
    this.lat2 = red_zone.lat2;
    };

RedZone.create = (newCapteur, result) => {
        sql.query("INSERT INTO red_zone SET ?", newCapteur, (err, res) => {
          if (err) {
            console.log("error: ", err);
            result(err, null);
            return;
          }
      
          console.log("created red_zone: ", { id: res.insertId, ...newCapteur });
          result(null, { id: res.insertId, ...newCapteur });
        });
      };
    

module.exports = RedZone;