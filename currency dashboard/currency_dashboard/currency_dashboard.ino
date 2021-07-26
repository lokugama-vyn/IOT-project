#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>

// Replace with your network credentials
const char* ssid     = "ssid";
const char* password = "password";
const char* mqtt_server = "Broker.mqttdashboard.com";
WiFiClient espClient;
AsyncWebServer server(80);
PubSubClient client(espClient);
// Set web server port number to 80

//to publish toipcs
char copy1[300];
char copy2[300];
char copy3[300];
char copy4[300];
char copy5[300];

//input parameters
const char* PARAM_INPUT_1 = "Name";
const char* PARAM_INPUT_2 = "preference";
const char* PARAM_INPUT_3 = "amount";
const char* PARAM_INPUT_4 = "weather";
const char* PARAM_INPUT_5 = "location";


//html webpage get inputs
String inputMessage1,inputMessage2,inputMessage3,inputMessage4,inputMessage5;
String inputParam1,inputParam2,inputParam3,inputParam4,inputParam5;
//incomming topics store
String incomming1="";
String incomming2="";
String incomming3="";
String incomming4="";




// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Currency rates and weather updates</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js" integrity="sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM" crossorigin="anonymous"></script>
  <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js" integrity="sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p" crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.min.js" integrity="sha384-cVKIPhGWiC2Al4u+LWgxfKTRIcfu0JTxR+EQDz/bgldoEyl4H0zUF0QKbrJ0EcQF" crossorigin="anonymous"></script>
  <style>
  h1{
  color:black;
  font-size:2em;
}

.titlediv{
  text-align:center;
  padding-top:20px;
  padding-bottom:20px;
  background-color:rgb(233, 66, 245);
  border-style:solid;
  background-image:url("https://t4.ftcdn.net/jpg/02/08/46/61/360_F_208466153_yKWoSWgUq71DamgtODhsTu9apfxQ12tq.jpg");
  background-repeat:no-repeat;
  height:220px;
  width:100%;
  background-position: center;
  background-size:cover;
}
.input_division{
  content: "";
  display: table;
  clear: both;
  font-style:initial;
  background-image:url("https://images.fineartamerica.com/images/artworkimages/mediumlarge/2/9-beautiful-photograph-of-the-great-wall-of-china-steve-clarke.jpg");
  height:560px;
  padding-top:90px;
  background-position: center;
  background-repeat: no-repeat;
  background-size:cover;
  position: relative;
  width:100%;
  font-size:20px;
  border-style:solid;
  color:black;
  border-color:black;
  
  
}

@media only screen and (min-width: 500px) {
  /* For tablets: */
  .col-s-1 {width: 8.33%;}
  .col-s-2 {width: 16.66%;}
  .col-s-3 {width: 25%;}
  .col-s-4 {width: 33.33%;}
  .col-s-5 {width: 41.66%;}
  .col-s-6 {width: 50%;}
  .col-s-7 {width: 58.33%;}
  .col-s-8 {width: 66.66%;}
  .col-s-9 {width: 75%;}
  .col-s-10 {width: 83.33%;}
  .col-s-11 {width: 91.66%;}
  .col-s-12 {width: 100%;}
}
@media only screen and (min-width: 768px) {
  /* For desktop: */
  .col-1 {width: 8.33%;}
  .col-2 {width: 16.66%;}
  .col-3 {width: 25%;}
  .col-4 {width: 33.33%;}
  .col-5 {width: 41.66%;}
  .col-6 {width: 50%;}
  .col-7 {width: 58.33%;}
  .col-8 {width: 66.66%;}
  .col-9 {width: 75%;}
  .col-10 {width: 83.33%;}
  .col-11 {width: 91.66%;}
  .col-12 {width: 100%;}
}

[class*="col-"] {
  float: left;
  padding: 15px;
}
@media only screen and (max-width: 768px) {
  /* For mobile phones: */
  [class*="col-"] {
    width: 100%;
  }
}
</style>
  </head><body>
  <div class="titlediv">
  <h1> CURRENCY,WEATHER AND COVID 19 UPDATES IN CHINA</h1><br>
  <input type="button" value="Group04" onclick= "alert('Welcome to our webpage..Hope you are keeping safe these pandemic time')" ></br>
  </div>
  <form action="/get">
  <div class ="input_division" ><br>
 
  <div class="col-4 col-s-12">
    <p><b>&#9635 What is your Name: <input type="text" name="Name">
    </p>
  </div>
  <div class="col-4 col-s-12">
  
    <p>&#9635 To what do you need to convert :</p>
    <input type="radio" id="dollars" name="preference" value="Dollars">
    <label for="dollars">Dollars</label><br>
    <input type="radio" id="renminbi" name="preference" value="Renminbi">
    <label for="renminbi">Renminbi</label><br>
    <p>&#9635 Enter the amount you wish to convert :</p> <input type="text" name="amount">
    
  </div>
  <div class="col-4 col-s-12">
    <p>&#9635 Do you need weather update:</p>
    <input type="radio" id="yes" name="weather" value="Yes">
    <label for="yes">Yes</label><br>
    <input type="radio" id="no" name="weather" value="No">
    <label for="no">No</label><br>
    <p>&#9635 If yes please enter the city name here:</p> <input type="text" name="location">
    <br><br><input type="submit" value="Submit"></br></br>
  </div>
  
  </form></b></div>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  // incomming topics
  
  if(strcmp(topic,"incommingCurrency")==0){
    incomming1="";                                        //clear existing data
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      incomming1+=(char)payload[i];
    }  
  }
   Serial.println(incomming1);
 if(inputMessage4=="Yes"){
  if(strcmp(topic,"incommingWeather")==0){
    incomming2="";
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      incomming2+=(char)payload[i];
    }
   }
  //Serial.println(incomming2);
   if(strcmp(topic,"incommingAirquality")==0){
    incomming3="";
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      incomming3+=(char)payload[i];
    }  
 }
   Serial.println(incomming3);
 }
 if(inputMessage4=="No"){
  incomming2="Say yes to get Weather updates of a particular city.";
  incomming3="Say yes to get Air Quality of a particular city.";
 }
   
 if(strcmp(topic,"incommingCovid")==0){
    incomming4="";
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      incomming4+=(char)payload[i];
    }  
 }
   //delay(1000);
   //Serial.println(incomming4);
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
            request->send(200, "text/html","<!DOCTYPE HTML><html><head>\n  <title>Currency rates and weather updates</title>\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n \n \n  <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC\" crossorigin=\"anonymous\">\n  <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM\" crossorigin=\"anonymous\"></script>\n  <script src=\"https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js\" integrity=\"sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p\" crossorigin=\"anonymous\"></script>\n<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.min.js\" integrity=\"sha384-cVKIPhGWiC2Al4u+LWgxfKTRIcfu0JTxR+EQDz/bgldoEyl4H0zUF0QKbrJ0EcQF\" crossorigin=\"anonymous\"></script>\n  \n  <style>.firstdiv{\n  text-align:center;\n  padding-top:20px;\n  background-color:rgb(233, 66, 245);\n  border-style:solid;\n  background-image:url(\"https://t4.ftcdn.net/jpg/02/08/46/61/360_F_208466153_yKWoSWgUq71DamgtODhsTu9apfxQ12tq.jpg\");\n  background-repeat:no-repeat;\n  height:230px;\n  width:100%;\n    border-bottom:none;\n    \n}\n.secondiv{\n  content: \"\";\n  display: table;\n  clear: both;\n  border-style:solid;\n}\n.column1{\n  border-style:solid;\n  color:black;\n  float:left;\n  width:25%;\n  height:610px;\n  background-image:url(\"https://i.pinimg.com/736x/78/b2/2d/78b22d4e15e7011694c9fa3ac3a062da.jpg\");\n }\n.column2{\n  border-style:solid;\n  color:black;\n  float:left;\n  width:25%;\n  height:610px;\n  background-image:url(\"https://cdn130.picsart.com/330892940013201.jpg?type=webp&to=min&r=640\");\n }\n.column3{\n  border-style:solid;\n  color:black;\n  float:left;\n  width:25%;\n  height:610px;\n  background-image:url(\"https://bloximages.newyork1.vip.townnews.com/coloradopolitics.com/content/tncms/assets/v3/editorial/c/ca/ccacb9ab-91f9-5ab3-9710-952baf9d6a73/5b997827895cf.image.jpg?resize=945%2C630\");\n }\n.column4{\n  border-style:solid;\n  color:black;\n  float:left;\n  width:23.7%;\n  height:610px;\n  background-image:url(\"https://www.dioceseaj.org/wp-content/uploads/2020/03/virus.png\");\n  background-repeat:no-repeat;\n }\nh2{\n  text-align:center;\n}\n    .currency{\n      font-size:30px;\n      color:black;\n      text-align:left;\n    }\n    .weather{\n      font-size:30px;\n      color:black;\n      text-align:left;\n    }\n    .air{\n      font-size:30px;\n      color:black;\n      text-align:left;\n    }\n    .covid{\n      font-size:30px;\n      color:black;\n      text-align:left;\n    }\n   \n\n@media only screen and (min-width: 500px) {\n  /* For tablets: */\n  .col-s-1 {width: 8.33%;}\n  .col-s-2 {width: 16.66%;}\n  .col-s-3 {width: 25%;}\n  .col-s-4 {width: 33.33%;}\n  .col-s-5 {width: 41.66%;}\n  .col-s-6 {width: 50%;}\n  .col-s-7 {width: 58.33%;}\n  .col-s-8 {width: 66.66%;}\n  .col-s-9 {width: 75%;}\n  .col-s-10 {width: 83.33%;}\n  .col-s-11 {width: 91.66%;}\n  .col-s-12 {width: 100%;}\n}\n@media only screen and (min-width: 768px) {\n  /* For desktop: */\n  .col-1 {width: 8.33%;}\n  .col-2 {width: 16.66%;}\n  .col-3 {width: 25%;}\n  .col-4 {width: 33.33%;}\n  .col-5 {width: 41.66%;}\n  .col-6 {width: 50%;}\n  .col-7 {width: 58.33%;}\n  .col-8 {width: 66.66%;}\n  .col-9 {width: 75%;}\n  .col-10 {width: 83.33%;}\n  .col-11 {width: 91.66%;}\n  .col-12 {width: 100%;}\n}\n\n[class*=\"col-\"] {\n  float: left;\n  padding: 15px;\n}\n@media only screen and (max-width: 768px) {\n  /* For mobile phones: */\n  [class*=\"col-\"] {\n    width: 100%;\n  }\n}</style>\n  \n  </head><body>\n  <div class=\"firstdiv\"><h1>CURRENCY,WEATHER AND COVID 19 UPDATES IN CHINA</h1>\n  \n \n    <h2>HI "+inputMessage1+" </h2>\n      <input type=\"button\" value=\"Press Here\" onClick=\"window.location.reload()\">\n    </div>\n  <div class=\"seconddiv\">\n    <div class=\"column1 col-3 col-s-12\">\n      <h2>CURRENCY DETAILS</h2>\n      <div class=\"currency\">\n        <br><br>\n"+incomming1+"      </div>\n    </div>\n    <div class=\"column2 col-3 col-s-12\">\n      <h2>WEATHER UPDATES</h2>\n      <div class=\"weather\">\n        <br><br>\n"+incomming2+"      </div>\n    </div>\n     <div class=\"column3 col-3 col-s-12\">\n      <h2>AIR QUALITY CHECK</h2>\n       <div class=\"air\">\n        <br><br>\n"+incomming3+"     </div>\n    </div>\n    <div class=\"column4 col-3 col-s-12\">\n      <h2>COVID 19 UPDATES</h2>\n      <div class=\"covid\">\n        <br><br>\n"+incomming4+"      </div>\n    </div>\n    ");
   });
   

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      client.subscribe("incommingCurrency");                          //subscribe topics
      client.subscribe("incommingWeather"); 
      client.subscribe("incommingAirquality"); 
      client.subscribe("incommingCovid");
      client.subscribe("incommingNoWeather"); 
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
     
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage1>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();                    //store username
      //Serial.println(inputMessage1);                                        
      inputParam1 = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();                    //publishing topics outgoingTopic2,outgoingTopic3,outgoingTopic4,outgoingTopic5
      //Serial.println(inputMessage2); 
      String myinput2=inputMessage2;                       
      myinput2.toCharArray(copy2,300);            
      //Serial.print("Publish message2:");              
      //Serial.println(copy2);
      client.publish("outgoingTopic2", copy2);
      delay(1000); 
      inputParam2 = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage3>
    if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage3 = request->getParam(PARAM_INPUT_3)->value();
      //Serial.println(inputMessage3);
      String myinput3=inputMessage3;                       
      myinput3.toCharArray(copy3,300);            
      //Serial.print("Publish message3:");              
      //Serial.println(copy3);
      client.publish("outgoingTopic3", copy3);
      delay(1000);                                      
      inputParam3 = PARAM_INPUT_3;
    }
    if (request->hasParam(PARAM_INPUT_4)) {
      inputMessage4 = request->getParam(PARAM_INPUT_4)->value();
      //Serial.println(inputMessage4);  
      String myinput4=inputMessage4;                       
      myinput4.toCharArray(copy4,300);            
      //Serial.print("Publish message4:");              
      //Serial.println(copy4);
      client.publish("outgoingTopic4", copy4);
      delay(1000);                           
      inputParam4 = PARAM_INPUT_4;
    }
    if (request->hasParam(PARAM_INPUT_5)) {
      inputMessage5 = request->getParam(PARAM_INPUT_5)->value();
      //Serial.println(inputMessage5); 
      
      String myinput5=inputMessage5;                       
      myinput5.toCharArray(copy5,300);            
      //Serial.print("Publish message5:");              
      //Serial.println(copy5);
      client.publish("outgoingTopic5", copy5);
      delay(1000); 
      inputParam5 = PARAM_INPUT_5;
      
    }
    
    delay(700); //wait for the output
    
    //output page html code
    request->send(200, "text/html","<!DOCTYPE HTML><html><head>\n  <title>Currency rates and weather updates</title>\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n \n \n  <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC\" crossorigin=\"anonymous\">\n  <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM\" crossorigin=\"anonymous\"></script>\n  <script src=\"https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js\" integrity=\"sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p\" crossorigin=\"anonymous\"></script>\n<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.min.js\" integrity=\"sha384-cVKIPhGWiC2Al4u+LWgxfKTRIcfu0JTxR+EQDz/bgldoEyl4H0zUF0QKbrJ0EcQF\" crossorigin=\"anonymous\"></script>\n  \n  <style>.firstdiv{\n  text-align:center;\n  padding-top:20px;\n  background-color:rgb(233, 66, 245);\n  border-style:solid;\n  background-image:url(\"https://t4.ftcdn.net/jpg/02/08/46/61/360_F_208466153_yKWoSWgUq71DamgtODhsTu9apfxQ12tq.jpg\");\n  background-repeat:no-repeat;\n  height:230px;\n  width:100%;\n    border-bottom:none;\n    \n}\n.secondiv{\n  content: \"\";\n  display: table;\n  clear: both;\n  border-style:solid;\n}\n.column1{\n  border-style:solid;\n  color:black;\n  float:left;\n  width:25%;\n  height:610px;\n  background-image:url(\"https://i.pinimg.com/736x/78/b2/2d/78b22d4e15e7011694c9fa3ac3a062da.jpg\");\n }\n.column2{\n  border-style:solid;\n  color:black;\n  float:left;\n  width:25%;\n  height:610px;\n  background-image:url(\"https://cdn130.picsart.com/330892940013201.jpg?type=webp&to=min&r=640\");\n }\n.column3{\n  border-style:solid;\n  color:black;\n  float:left;\n  width:25%;\n  height:610px;\n  background-image:url(\"https://bloximages.newyork1.vip.townnews.com/coloradopolitics.com/content/tncms/assets/v3/editorial/c/ca/ccacb9ab-91f9-5ab3-9710-952baf9d6a73/5b997827895cf.image.jpg?resize=945%2C630\");\n }\n.column4{\n  border-style:solid;\n  color:black;\n  float:left;\n  width:23.7%;\n  height:610px;\n  background-image:url(\"https://www.dioceseaj.org/wp-content/uploads/2020/03/virus.png\");\n  background-repeat:no-repeat;\n }\nh2{\n  text-align:center;\n}\n    .currency{\n      font-size:30px;\n      color:black;\n      text-align:left;\n    }\n    .weather{\n      font-size:30px;\n      color:black;\n      text-align:left;\n    }\n    .air{\n      font-size:30px;\n      color:black;\n      text-align:left;\n    }\n    .covid{\n      font-size:30px;\n      color:black;\n      text-align:left;\n    }\n   \n\n@media only screen and (min-width: 500px) {\n  /* For tablets: */\n  .col-s-1 {width: 8.33%;}\n  .col-s-2 {width: 16.66%;}\n  .col-s-3 {width: 25%;}\n  .col-s-4 {width: 33.33%;}\n  .col-s-5 {width: 41.66%;}\n  .col-s-6 {width: 50%;}\n  .col-s-7 {width: 58.33%;}\n  .col-s-8 {width: 66.66%;}\n  .col-s-9 {width: 75%;}\n  .col-s-10 {width: 83.33%;}\n  .col-s-11 {width: 91.66%;}\n  .col-s-12 {width: 100%;}\n}\n@media only screen and (min-width: 768px) {\n  /* For desktop: */\n  .col-1 {width: 8.33%;}\n  .col-2 {width: 16.66%;}\n  .col-3 {width: 25%;}\n  .col-4 {width: 33.33%;}\n  .col-5 {width: 41.66%;}\n  .col-6 {width: 50%;}\n  .col-7 {width: 58.33%;}\n  .col-8 {width: 66.66%;}\n  .col-9 {width: 75%;}\n  .col-10 {width: 83.33%;}\n  .col-11 {width: 91.66%;}\n  .col-12 {width: 100%;}\n}\n\n[class*=\"col-\"] {\n  float: left;\n  padding: 15px;\n}\n@media only screen and (max-width: 768px) {\n  /* For mobile phones: */\n  [class*=\"col-\"] {\n    width: 100%;\n  }\n}</style>\n  \n  </head><body>\n  <div class=\"firstdiv\"><h1>CURRENCY,WEATHER AND COVID 19 UPDATES IN CHINA</h1>\n  \n \n    <h2>HI "+inputMessage1+" </h2>\n      <input type=\"button\" value=\"Press Here\" onClick=\"window.location.reload()\">\n    </div>\n  <div class=\"seconddiv\">\n    <div class=\"column1 col-3 col-s-12\">\n      <h2>CURRENCY DETAILS</h2>\n      <div class=\"currency\">\n        <br><br>\n"+incomming1+"      </div>\n    </div>\n    <div class=\"column2 col-3 col-s-12\">\n      <h2>WEATHER UPDATES</h2>\n      <div class=\"weather\">\n        <br><br>\n"+incomming2+"      </div>\n    </div>\n     <div class=\"column3 col-3 col-s-12\">\n      <h2>AIR QUALITY CHECK</h2>\n       <div class=\"air\">\n        <br><br>\n"+incomming3+"     </div>\n    </div>\n    <div class=\"column4 col-3 col-s-12\">\n      <h2>COVID 19 UPDATES</h2>\n      <div class=\"covid\">\n        <br><br>\n"+incomming4+"      </div>\n    </div>\n    ");

  });
  server.onNotFound(notFound);
  server.begin();
  
}
  


 


void loop(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  

  
  
}
  
