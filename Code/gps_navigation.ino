#include <TinyGPS.h> //allows breakdwon of data retrieved from gps device
#include <SoftwareSerial.h>
#define _USE_MATH_DEFINES
#include <math.h>

SoftwareSerial GPS(2,5);

TinyGPS gpsShield; //create an instance of gps shield

//Create a structure to hold variables globally
struct variables {
  //Define variables for longitude and latitude in decimal form
  float longitude;
  float latitude;
  float bearing;
};

const float pi = atan(1)*4;

int iteration = 0;


void setup() {
  GPS.begin(9600);
  Serial.begin(9600);

  //Setup Channel A for Motor 1
  pinMode(12, OUTPUT); //Inititates Motor Channel A pin
  pinMode(9, OUTPUT); //Inititates Brake Channel A pin

  //Setup Channel B for Motor 2
  pinMode(13, OUTPUT); //Inititates Motor Channel B pin
  pinMode(8, OUTPUT); //Inititates Brake Channel B pin
}

void loop()
{
  if (iteration<4) {
    byte gpsData;
    if ( GPS.available() > 0 ) // if there is data coming from the GPS shield
    {
      gpsData = GPS.read(); // get the byte of data
      if(gpsShield.encode(gpsData)) // if there is valid GPS data...
      
      {
        //Serial.write(gpsData);
        setMotors(HIGH,100,LOW,HIGH,100,LOW);
        delay(3000);
        setMotors(HIGH,0,LOW,HIGH,0,LOW);
        struct variables currentPositionDecimal = breakDownGpsData(gpsShield);
        runMotors(currentPositionDecimal,0.001,90);
        iteration++;
      }
    }
  }
  
}

struct variables breakDownGpsData(TinyGPS &gps)
{
  //Define variables for holding lat, long and bearing
  float currentLongitude, currentLatitude, currentBearing;

  gpsShield.f_get_position(&currentLatitude, &currentLongitude);
  currentBearing = gpsShield.course();

  //Store the current gps data
  struct variables currentLatLongBea;
  currentLatLongBea.latitude = currentLatitude;
  currentLatLongBea.longitude = currentLongitude;
  currentLatLongBea.bearing = currentBearing;

  //convert and store the current position to decimal values
  struct variables decimalCurrentPosition = conversionDecimalLatLong(currentLatitude,currentLongitude, currentBearing,0);


  
  return decimalCurrentPosition;
}

struct variables conversionDecimalLatLong(float latitude, float longitude, float bearing,int decimalOrLatLong) //decimalOrLatLong is 0 for converting to decimal and 1 for the inverse
{
  if(decimalOrLatLong ==0) {
    struct variables decimalVariables;

    //convert NMEA data to decimal
    float latitudeDecimal = floor(latitude/100) + fmod(latitude,100)/60;
    float longitudeDecimal = floor(longitude/100) + fmod(longitude,100)/60;

    //store and return to structure
    decimalVariables.longitude = longitudeDecimal;
    decimalVariables.latitude = latitudeDecimal;
    decimalVariables.bearing = bearing;
    return decimalVariables;
  }
  else {
    struct variables NMEAVariables;
    
    //convert to NMEA data
    float latitudeNMEA = (floor(latitude)+fmod(latitude,1)*60)*100;
    float longitudeNMEA = (floor(longitude)+fmod(longitude,1)*60)*100;

    //store and return to structure
    NMEAVariables.latitude = latitudeNMEA;
    NMEAVariables.longitude = longitudeNMEA;
    NMEAVariables.bearing = bearing;
    return NMEAVariables;
    }
}


struct variables getWaypoint(variables currentPosition, float distanceKM, int angleChange)
{
  //convert all variables from degrees to radians
  float latitudeRadians = degreeRadianConversion(currentPosition.latitude,0);
  float longitudeRadians = degreeRadianConversion(currentPosition.longitude,0);
  float oldBearingRadians = degreeRadianConversion(currentPosition.bearing,0);
  float newBearingRadians;
  if (oldBearingRadians+angleChange>=2*pi) {
    newBearingRadians = oldBearingRadians + angleChange - 2*pi;
  }
  else {
    newBearingRadians = oldBearingRadians + angleChange;
  }

  //calculate the radius of the earth at this position
  float earthRadius = pow(pow(6356.7523*sin(latitudeRadians),2)+ pow(6378.137*cos(latitudeRadians),2),1/2);

  //calculate new latitude and longitude
  float newLatitudeRadians = asin(sin(latitudeRadians)*cos(distanceKM/earthRadius) + cos(latitudeRadians)*sin(distanceKM/earthRadius)*cos(newBearingRadians));
  float newLongitudeRadians = longitudeRadians + atan2(sin(newBearingRadians)*sin(distanceKM/earthRadius)*cos(latitudeRadians),cos(distanceKM/earthRadius) - sin(latitudeRadians)*sin(newLatitudeRadians));

  //convert back to degrees
  float newLatitude = degreeRadianConversion(newLatitudeRadians,1);
  float newLongitude = degreeRadianConversion(newLongitudeRadians,1);
  float newBearing = degreeRadianConversion(newBearingRadians,1);

  struct variables newPosition;

  //convert back to NMEA data
  newPosition = conversionDecimalLatLong(newLatitude,newLongitude,newBearing,1);
  
  return newPosition;
};

float degreeRadianConversion(float number, int degreeOrRadian) //degreeOrRadian is 0 if degree and 1 if radian
{
  if (degreeOrRadian == 0) {
    return number*pi/180;
  }
  if (degreeOrRadian == 1) {
    return number*180/pi;
  }
  else {
    return 0;
  }
}

void runMotors(variables currentPos,float distanceKM, float angleChange) {
  //obtain new waypoint
  struct variables waypoint = getWaypoint(currentPos, distanceKM, angleChange);

  //turn to face waypoint
  if (currentPos.bearing+angleChange<360) {
    while ( waypoint.bearing > currentPos.bearing) {
    setMotors(HIGH,100,LOW,LOW,100,LOW);
      }
    }
  if (currentPos.bearing+angleChange>360) {
    while (waypoint.bearing<currentPos.bearing) {
      setMotors(LOW,100,LOW,HIGH,100,LOW);
    }
  }


  if (fabs(currentPos.latitude-waypoint.latitude) > 0.0001 && fabs(currentPos.longitude-waypoint.longitude)>0.0001 ) {
    setMotors(HIGH,0,LOW,HIGH,0,LOW);
  }
  else {
    setMotors(HIGH,100,LOW,HIGH,100,LOW);
  }
}


//void to turn motors on and off, and vary speed
void setMotors(uint8_t channelADirection, int channelASpeed,uint8_t channelABrake,uint8_t channelBDirection, int channelBSpeed,uint8_t channelBBrake) {
  //set motor 1/channel A
  digitalWrite(12,channelADirection);
  digitalWrite(9,channelABrake);
  analogWrite(3,channelASpeed);

  //set motor 2/channel B
  digitalWrite(13,channelBDirection);
  digitalWrite(8,channelBBrake);
  analogWrite(11,channelBSpeed);
}



