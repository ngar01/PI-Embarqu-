module.exports = app => {
    const capteur = require("../controllers/capteur.controller.js");
  
    var router = require("express").Router();
  
    // Create a new Tutorial
    app.get("/getPosition", capteur.findAllPos);

    app.get("/getRedZone", capteur.findRedZone);
  
    // Retrieve all capteur
    app.get("/addPosition/:idUser/:long/:lat/:type", capteur.create);

    

    app.get("/addRedZone/:long1/:lat1/:long2/:lat2", capteur.createRedZone);
  
  
   app.use('/api/capteur', router);
  };