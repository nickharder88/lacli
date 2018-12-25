// npm modules
const express = require('express');
const uuid = require('uuid/v4');

// session - essentially the text string stored on the server that matches
//           the cookie sent by the client
const session = require('express-session');
const RedisStore = require('connect-redis')(session);
const bodyParser = require('body-parser');
//const path = require('path');

// create server
const app = express();

// add and configure middleware
app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json());
app.use(session({
  genid: (req) => {
    return uuid();
  },
  // replace with randomly generated string pulled from environment variable
  store: new RedisStore({hostname: "redis_store"}),
  secret: 'keyboard cat',
  resave: false,
  saveUninitialized: true,
}));

app.get('/', (req, res) => {
});

app.post('/', (req, res) => {
  console.log(req.session.input);
});

// tell the server what port to listen on
app.listen(3000, () => {
  console.log('listening on localhost:3000');
});
