const Capteur = require("../models/capteur.model.js");
const RedZone = require("../models/redzone.model.js");

// Create and Save a new capteur
exports.create = (req, res) => {
    // Create a capteur
  const capteur = new Capteur({
    id_user: req.params.idUser,
    longitude: req.params.long ,
    latitude: req.params.lat,
    type: req.params.type
  });

  // Save capteur in the database
  Capteur.create(capteur, (err, data) => {
    if (err)
      res.status(500).send({
        message:
          err.message || "Some error occurred while creating the capteur."
      });
    else res.send(data);
  });
  
};

exports.createRedZone = (req, res) => {
  // Create a c
const red_zone = new RedZone({
  long1: req.params.long1,
  lat1: req.params.lat1 ,
  long2: req.params.long2,
  lat2: req.params.lat2
});

// Save capteur in the database
RedZone.create(red_zone, (err, data) => {
  if (err)
    res.status(500).send({
      message:
        err.message || "Some error occurred while creating the redZone."
    });
  else res.send(data);
});

};

// Retrieve all capteurs from the database (with condition).
    exports.findAllPos = (req, res) => {
        Capteur.getAllPublished((err, data) => {
          if (err)
            res.status(500).send({
              message:
                err.message || "Some error occurred while retrieving capteur."
            });
          else res.send(data);
        });
      };


      exports.findRedZone = (req, res) => {
        Capteur.getRedZone((err, data) => {
          if (err)
            res.status(500).send({
              message:
                err.message || "Some error occurred while retrieving red_zone."
            });
          else res.send(data);
        });
      };
