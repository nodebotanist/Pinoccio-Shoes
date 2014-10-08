var secrets = require('./config/secrets'),
    pinoccio = require('pinoccio'),
    twit = require('twitter'),
    util = require('util');

var pinoccio_api = pinoccio(secrets.pinoccio_token);

var twitter_api = new twit({
  consumer_key: secrets.twitter_consumer,
  consumer_secret: secrets.twitter_consumer_secret,
  access_token_key: secrets.twitter_access,
  access_token_secret: secrets.twitter_access_secret
})

var troop_id,
    lefty_id,
    righty_id;

pinoccio_api.rest({url:"/v1/troops"},function(error,data){
  if(error) return console.log('oh no. i got an error getting my troops!',error);
  console.log('troop data', data);
  troop_id = data[0].id;

  pinoccio_api.rest({
    url: '/v1/' + troop_id + '/scouts'
  }, function(error, data){
    console.log('scouts data', data);
    lefty_id = data[0].id;
    righty_id = data[1].id;

    twitter_api.stream('user', function(stream) {
        stream.on('favorite', function(data) {
            console.log('favorited');
            pinoccio_api.rest({
              url: 'v1/' + troop_id + '/'+ lefty_id + '/command',
              data: {
                command: 'pixels.flash(255, 255, 0)'
              }
            }, function(){});

            pinoccio_api.rest({
              url: 'v1/' + troop_id + '/'+ righty_id + '/command',
              data: {
                command: 'pixels.flash(255, 255, 0)'
              }
            }, function(){})
        });
    });
  });
});
