#include "drawingFunctions.cpp"

//Blocks upon which the world may rest
class block{
	int x,y;
    public:
	void init(int xi,int yi){x=xi;y=yi;}
	int posx(){return x;}
	int posy(){return y;}
	void draw(Pixmap pixmap, XInfo &xinfo){	//NOTE: 'pixmap' is only used for double buffering, if applicable
		if (x>-50 && x<800)
			drawBlock(xinfo, xinfo.gc[0], x,y);
	}
	void move(){x-=10;}//scroll from right to left
};

//Sun/Moon. Alternates between the two
class sunMoon{
	int x,y,speed;
	bool day;	//determines whether to draw a sun or a moon
    public:
	void init(int xi,int yi, int movementSpeed){x=xi;y=yi;speed=movementSpeed;day=true;}
	int posx(){return x;}
	int posy(){return y;}
	void draw(Pixmap pixmap, XInfo &xinfo){	//NOTE: 'pixmap' is only used for double buffering, if applicable
		if (x>-50 && x<800){
			if (day) drawSun(xinfo, xinfo.gc[0], x,y);
			else drawMoon(xinfo, xinfo.gc[0], xinfo.gc[2], x,y);
		}
	}
	
	//if sun/moon is too far off screen, moved to other side of screen as night turns to day, and day to night
	void tooFar(){
		if (x> 1000){
			day = (day+1) % 2;
			x = -100; 
		}
	}
	void move(){x+=speed;}//scroll from left to right
	bool isDay(){
		return day;
	}
};

//It's a stickman. Not Mario.
class notMario{
	int x,y,speed, jumpSpeed, counter, invulnCounter, airTime, score,trueScore,xDest,yDest,xTemp,yTemp;
	double xDir,yDir,maxSpeed;
	bool leftFace, rightFace, running, hurt, flowerPower,onTheGround;
    public:
	void init(int xi, int yi, int movementSpeed, int jumpingSpeed){x=xi;y=yi;speed=movementSpeed;jumpSpeed=jumpingSpeed;leftFace = false; rightFace = false; running = false; hurt = false; flowerPower=false; counter=0; airTime=0;score=100;trueScore=0;xDest=-1;yDest=-1;xDir=0;yDir=0;maxSpeed=(speed*1.5);}
	int posx(){return x;}
	int posy(){return y;}
	int getScore(int type){if (type==0) return score; else return trueScore;}
	void draw(Pixmap pixmap, XInfo &xinfo){ //NOTE: 'pixmap' is only used for double buffering, if applicable
		GC gc = xinfo.gc[0];

		if (y>=650) return;

		if (flowerPower) gc=xinfo.gc[1];	//not-Mario is drawn bold to indicate ability to withstand damage		
		
		if (hurt && invulnCounter%5==0){	//not-Mario will not be drawn 1/5 times while hurt, to indicate invulnerability
			if (flowerPower && invulnCounter==20) flowerPower=false;
			else if (invulnCounter==35) hurt=false;
		}
		else if (airTime != 0){			//not-Mario has been hurt, and thus faces the screen and jumps
			drawMarioStill(xinfo, gc, x, y-(5-airTime));
			airTime--;
		} 
		else if (!leftFace && !rightFace) drawMarioStill(xinfo, gc, x, y);
		else if (leftFace){
			if (running) drawMarioRunLeft(xinfo, gc, x, y, counter);
			else drawMarioLeft(xinfo, gc, x, y);
		}
		else if (rightFace){
			if (running) drawMarioRunRight(xinfo, gc, x, y, counter);
			else drawMarioRight(xinfo, gc, x, y);
		}
		if (hurt) invulnCounter++;
	}
	void moveLeft(){
		rightFace=false;
		leftFace=true;
		running=true;

		counter= (counter+1)%2;
		if (x>0 && xDir>(0-maxSpeed)) xDir -=(speed/3.5);
	}
	void moveRight(){
		leftFace=false;
		rightFace=true;
		running=true;
		
		counter= (counter+1)%2;
		if (x<750 && xDir<maxSpeed) xDir+=(speed/3);
	}
	void moveOther(){
		leftFace=false;
		rightFace=false;
		running=false;
	}
	void runStop(){running=false; counter=0;}

	//Determines if a collision has occurred
	bool hit(int posx, int posy, int width, int height){
		if ((x+15)<(posx+width) && ((x+15)+20) > posx){		//NOTE: (x+15,y+5) corresponds to the top left position of the notMario drawing, (20,45) is its (width, height)
			if ((y+5)<(posy+height) && ((y+5)+45) > posy){
				return true;
			}
		}
		return false;
	}

	//Called if not-Mario has collided with an enemy
	void isHurt(){
		if (hurt){}
		else{
			hurt = true;
			if (!flowerPower){ airTime=5; score -=100; }
			invulnCounter=0;
		}
	}

	//Called to determine if not-Mario is doing his 'hurting animation'. Used to prevent player input while animation occurs
	bool hurting(){
		if (airTime!=0) return true;
		return false;
	}
	bool invulnerable(){
		return hurt;
	}
	void itemPickup(int type){	//such that type=... 0=>coin, 1=>flower, 2=>mushroom
		if (type==2){
			if (rand()%2 == 0) score-=50;	//mushroom has a 1/3 chance of being poisonous. A poison mushroom damages damages score.
			else type=1;
		}
		if (type==1){
			if (flowerPower&&!hurt) score+=100;
			flowerPower = true;
			if (hurt) invulnCounter= 21;
		}
		else if (type==0){
			score+=100;
		}
	}
	void newJump(){
		if (onTheGround){
			yDir = 0-(100/6);
		}
	}
	void update(){
		onTheGround=false;	//since update is called before any block collision statements, not-Mario is assumed in the air unless proven otherwise
		y+=yDir;
		y+=jumpSpeed;
		if (x+xDir >= 0 && x+xDir <= 750) x+=xDir;
		if (xDir>0) xDir -= 0.5;
		if (xDir<0) xDir += 0.5;
		//if (yDir<(jumpSpeed*2)) yDir++;
		if (yDir<jumpSpeed) yDir++;
		if (y>600) {score = -1;y=650;}
	}

	//Repositions not-Mario after a collision with a block or the like. Due to the simplistic nature of this function, if not Mario's running or jumping speed are set too high there is a risk of not-Mario moving through blocks
	void reposition(int blockX, int blockY){
		if (y < blockY && (y-(2*jumpSpeed)) <= (blockY-50)){
			y=blockY-50;
			onTheGround=true;	//not Mario is most likely now on the ground
		}
		else if (x < blockX) {x=(blockX-30)-5;}
		else if (x > blockX) {x=(blockX+30)+5;}
	}
	void gameOver(){
		leftFace =false;
		rightFace=false;
		score=0;
		y-=50;
	}
	void move(){x-=10;}//scroll from right to left
	void levelUp(){
		trueScore+=score;
		score=100;
	}
};

//Item. Colliding with a coin increases score
class coin{
	int x,y;
    public:
	void init(int xi,int yi){x=xi;y=yi;}
	int posx(){return x;}
	int posy(){return y;}
	void draw(Pixmap pixmap, XInfo &xinfo){	//NOTE: 'pixmap' is only used for double buffering, if applicable
		if (x>-50 && x<800)
			drawCoin(xinfo, xinfo.gc[0], x,y);
	}
	void move(){x-=10;}//scroll from right to left
};

//Enemy. Colliding with a spike decreases score
class spike{
	int x,y;
    public:
	void init(int xi,int yi){x=xi;y=yi;}
	int posx(){return x;}
	int posy(){return y;}
	void draw(Pixmap pixmap, XInfo &xinfo){	//NOTE: 'pixmap' is only used for double buffering, if applicable
		if (x>-50 && x<800)
			drawSpikes(xinfo, xinfo.gc[0], x,y);
	}
	void move(){x-=10;}//scroll from right to left
};

//Enemy. Colliding with a bullet decreases score
class bullet{
	int x,y,parentX;
	bool right, hitBlock;
    public:
	void init(int xi,int yi,bool notLeft){x=xi;y=yi;parentX=xi;right=notLeft;}
	int posx(){return x;}
	int posy(){return y;}
	void draw(Pixmap pixmap, XInfo &xinfo){	//NOTE: 'pixmap' is only used for double buffering, if applicable
		if (x>-50 && x<800 && y>0){
			drawBullet(xinfo, xinfo.gc[0], xinfo.gc[2],x,y,right);
		}
	}
	void move(int difficulty){
		if (x>=parentX-800 && x<=parentX+800){
			if (right) x+=(5+difficulty);
			else x-=(5+difficulty);
		}
	}

	//Determines if bullet has collided with a given block. If so, bullet is moved off screen
	void blockCollision(int posx, int posy, int width, int height){
		if ((x+10)<(posx+width) && ((x+10)+40) > posx){
			if ((y+2)<(posy+height) && ((y+2)+11) > posy){
				y=-1;
			}
		}
	}
};

//Block. Bill spawns bullets
class Bill{
	int x,y;
	bullet myBullet;
    public:
	void init(int xi,int yi){x=xi;y=yi;myBullet.init(x+400,-1,true);}
	int posx(bool bill){if (bill) return x; else return myBullet.posx();}
	int posy(bool bill){if (bill) return y; else return myBullet.posy();}
	void draw(Pixmap pixmap, XInfo &xinfo){	//NOTE: 'pixmap' is only used for double buffering, if applicable
		if (x>-50 && x<800){
			myBullet.draw(pixmap, xinfo);
			drawBill(xinfo, xinfo.gc[0], x,y);
		}
	}

	//Spawns bullet at Bill's location facing not-Mario if bullet is certain distance from Bill. Otherwise moves bullet forwards 
	void FIRE(int MarioPosx, int difficulty){
		if (myBullet.posx() <= x-800 || myBullet.posx() >= x+800){
			if (MarioPosx > x+30) 			
				myBullet.init(x,y,true);
			else if (MarioPosx < x-30)
				myBullet.init(x,y,false);
			else myBullet.init(x+400,-1,true);
		}
		else myBullet.move(difficulty);
	}
	void bulletCollide(int posx, int posy, int width, int height){
		myBullet.blockCollision(posx, posy, width, height);
	}
	void move(){
		x-=10; 
		myBullet.init(x+600,-1,true);
	}
};

//Item. Colliding with a flower gives not-Mario the ability to withstand to withstand a single hit from an enemy. Flowers spawn while the sun is out
class flower{
	int x,y;
    public:
	void init(int xi,int yi){x=xi;y=yi;}
	int posx(){return x;}
	int posy(){return y;}
	void draw(Pixmap pixmap, XInfo &xinfo){	//NOTE: 'pixmap' is only used for double buffering, if applicable
		if (x>-50 && x<800)
			drawFlower(xinfo, xinfo.gc[0], x,y);
	}
	void move(){x-=10;}//scroll from right to left
};

//Item. Colliding with a mushroom has either the same effect as the flower, or reduces not-Mario's score. Mushrooms spawn while the moon is out
class mushroom{
	int x,y;
    public:
	void init(int xi,int yi){x=xi;y=yi;}
	int posx(){return x;}
	int posy(){return y;}
	void draw(Pixmap pixmap, XInfo &xinfo){	//NOTE: 'pixmap' is only used for double buffering, if applicable
		if (x>-50 && x<800)
			drawMushroom(xinfo, xinfo.gc[0], xinfo.gc[2], x,y);
	}
	void move(){x-=10;}//scroll from right to left
};

//Superfluous. endFlag indicates the end of a 'level'
class endFlag{
	int x,y;
    public:
	void init(int xi, int yi){x=xi;y=yi;}
	int posx(){return x;}
	int posy(){return y;}
	void draw(Pixmap pixmap, XInfo &xinfo){
		if (x>-50 && x<800)
			drawFlag(xinfo, xinfo.gc[0], x, y);
	}
	void move(){x-=10;}//scroll from right to left
};
