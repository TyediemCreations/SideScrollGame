#include "classFunctions.cpp"


//Environment. Objects are set up here.
class world{
	block *blocks;
	block *blocksTemp;
	notMario &Mario;
	sunMoon &theSun;
	coin &coins;
	flower &flowers;
	mushroom &mushrooms;
	spike &spikes;
	Bill &bBill;
	endFlag &flag;
	int MarioMovementSpeed,MarioJumpingSpeed,sunMovementSpeed,numScreens,difficulty,numBlocks,numBlocksNext;
	bool levelMove;
    public:
	world(block *blocks, block *blocksTemp, notMario &Mario, sunMoon &theSun, coin &coins, flower &flowers, mushroom &mushrooms, spike &spikes, Bill &bBill, int MarioMovementSpeed, int MarioJumpingSpeed, int sunMovementSpeed, endFlag &flag) : blocks(blocks), blocksTemp(blocksTemp), Mario(Mario), theSun(theSun), coins(coins), flowers(flowers), mushrooms(mushrooms), spikes(spikes), bBill(bBill), MarioMovementSpeed(MarioMovementSpeed), MarioJumpingSpeed(MarioJumpingSpeed), sunMovementSpeed(sunMovementSpeed), flag(flag) {
		numScreens=0;
		difficulty=1;
		levelMove=false;
	}

	void init();
	void nextCreate();
	void next();
	void nextSet();
	void draw(Pixmap pixmap, XInfo &xinfo);
	bool update(bool gameOver);
};

//Sets up inital positions of blocks, not-Mario, sun/moon, etc. Function returns the number of blocks of the 48 available that are used on-screen
void world::init(){
	int i=0;
	blocks[i].init(i,500);
	blocksTemp[i].init(i,500);
	int row, column, columnHeight, randomNum, heightOfLast=1;
	i=1;

	numScreens=0;
	difficulty=1;
	levelMove=false;		

	Mario.init(0, 400, MarioMovementSpeed, MarioJumpingSpeed);
	theSun.init(0,100,sunMovementSpeed);
	flag.init(-100,-100);

	int coinLocal,flushroomLocal,spikeLocal,BillLocal;	
	
	//a coin, mushroom/flower, spike and Bill are randomly given a position indicator for the current screen. Position indicators are analgous to the 2nd-15th rows of blocks (of 16)	
	coinLocal=rand()%14;
	flushroomLocal=rand()%14;
	while (flushroomLocal==coinLocal){flushroomLocal=rand()%14;}
	spikeLocal=rand()%14;
	BillLocal=rand()%14;
	while (BillLocal==spikeLocal || BillLocal==coinLocal || BillLocal==flushroomLocal){BillLocal=rand()%14;}
	
	//blocks are initiated to create the screen's floor, such that no block column is more than 1 block-height greater than the last. To create more level ground each column of blocks has around a 1/3 chance of being the same height as the one before. Items are initiated at their predetermined positions on top of the block columns
	for (row=1;row<15;row++){
		randomNum=rand()%3;
		if (randomNum==0) columnHeight=heightOfLast;
		else {
			columnHeight=rand()%(heightOfLast+1)+1;	
			if (columnHeight>4) columnHeight=4;	//max height of block columns is 4 to ensure that even if the max number of blocks is used per column, the 48 blocks will still be enough to create a traversable level		
		}

		if ((row-1)==coinLocal) {
			coins.init(row*50,500-(columnHeight*50));
		}
		if ((row-1)==flushroomLocal){
			if (theSun.isDay()){
				flowers.init(row*50,500-(columnHeight*50));
				mushrooms.init(-100,-100);
			}
			else{ 
				mushrooms.init(row*50,500-(columnHeight*50));
				flowers.init(-100,-100);
			}
		}
		if ((row-1)==spikeLocal){
			spikes.init(row*50,500-(columnHeight*50));
		}
		if ((row-1)==BillLocal){
			if (columnHeight<=1) columnHeight=2; 	//prevents impossible-to-jump-over scenarios given that Bill acts as a block
			bBill.init(row*50,500-((columnHeight-1)*50));
			columnHeight--;
		}
		heightOfLast=columnHeight;

		for (column=0;column<columnHeight;column++){
			if (i>46) break;
			blocks[i].init(row*50,500-(column*50));
			blocksTemp[i].init(row*50,500-(column*50));
			i++;
		}
	}
	row = 15;
	blocks[i].init(row*50, 500);
	blocksTemp[i].init(row*50, 500);
	i++;
	numBlocks=i;
	for (;i<48;i++){	//unused blocks are initiated to an irrelevant position
		blocks[i].init(-100,-100);
		blocksTemp[i].init(-100,-100);
	}
	
}

//Sets up new positions of blocks, coin, spike, etc. to be used on the upcoming screen. Function returns the number of blocks of the 48 available that will be used for the next screen
void world::nextCreate(){
	numScreens++;
	if (numScreens == 2) flag.init(750+750,300);
	if (numScreens == 3){	//not-Mario has reached an end flag. not-Mario's score is updated, and difficulty is increased (Bullets move faster and blocks tend towards more bottomless pits)
		Mario.levelUp();
		numScreens=0;
		difficulty++;
	}

	int i=0;
	blocks[i].init(750,500);
	int row, column, columnHeight, randomNum, heightOfLast=1;
	i=1;

	int coinLocal,flushroomLocal,spikeLocal,BillLocal;	
		
	coinLocal=rand()%14;
	flushroomLocal=rand()%14;
	while (flushroomLocal==coinLocal){flushroomLocal=rand()%14;}
	spikeLocal=rand()%14;
	BillLocal=rand()%14;
	while (BillLocal==spikeLocal || BillLocal==coinLocal || BillLocal==flushroomLocal)
		{BillLocal=rand()%14;}
	
	for (row=1;row<15;row++){
		randomNum=rand()%3;
		if (randomNum==0 && heightOfLast != 0) columnHeight=heightOfLast;
		else {
			if (heightOfLast==0) columnHeight=rand()%(heightOfLast+1)+1;	//prevents bottomless pits of length > 1 (i.e., not traversable)
			else columnHeight=rand()%(heightOfLast+2);
			if (columnHeight>4) columnHeight=4;
			else if (columnHeight==0){
				if (row==BillLocal) columnHeight=1;
				else {
					randomNum=rand()%difficulty;
					if (randomNum==0) columnHeight=1;	//higher difficulties => more bottomless pits
				}		
			}
		}
		if ((row-1)==coinLocal) {
			if (columnHeight==0) columnHeight=1;
			coins.init((row*50)+750,500-(columnHeight*50));
		}
		if ((row-1)==flushroomLocal){
			if (columnHeight==0) columnHeight=1;
			if (theSun.isDay()) {
				flowers.init((row*50)+750,500-(columnHeight*50));
				mushrooms.init(-100,-100);
			}
			else{ 
				mushrooms.init((row*50)+750,500-(columnHeight*50));
				flowers.init(-100,-100);
			}
		}
		if ((row-1)==spikeLocal){
			if (columnHeight==0) columnHeight=1;
			spikes.init((row*50)+750,500-(columnHeight*50));
		}
		if ((row-1)==BillLocal){
			if (columnHeight<=1) columnHeight=2; 
			bBill.init((row*50)+750,500-((columnHeight-1)*50));
			columnHeight--;
		}
		heightOfLast=columnHeight;
		for (column=0;column<columnHeight;column++){
			if (i>46) break;
			blocks[i].init((row*50)+750,500-(column*50));
			i++;
		}
		
	}
	row = 15;
	blocks[i].init((row*50)+750, 500);
	i++;
	numBlocksNext=i;
	for (;i<48;i++){
		blocks[i].init(-100,-100);
	}

}

//Moves all objects on screen to the left
void world::next(){
	int i;
	for (i=0;i<48;i++){
		blocks[i].move();
		blocksTemp[i].move();
	}
	Mario.move();
	coins.move();
	flowers.move();
	mushrooms.move();
	spikes.move();
	bBill.move();
	flag.move();
}

//Sets the temparary blocks to the blocks current blocks. Temparary blocks are used to maintain an image of the previous level as the new level is revealed
void world::nextSet(){
	for (int i=0;i<48;i++){
		blocksTemp[i]=blocks[i];
	}
}

//Draws all environment objects to the window
void world::draw(Pixmap pixmap, XInfo &xinfo){
	theSun.draw(pixmap, xinfo);
	bBill.draw(pixmap, xinfo);
	flowers.draw(pixmap, xinfo);
	mushrooms.draw(pixmap, xinfo);
	coins.draw(pixmap, xinfo);
	spikes.draw(pixmap, xinfo);
	flag.draw(pixmap, xinfo);

	for (int i=0;i<numBlocks;i++){
		blocks[i].draw(pixmap, xinfo);
		if (levelMove) blocksTemp[i].draw(pixmap, xinfo);
	}
			
	Mario.draw(pixmap, xinfo);
	Mario.runStop();
}

//Updates all environments objects (moves objects as needed, performs collision detection, etc). Returns whether or not the level is moving, and thus whether or not directional input should be accepted 
bool world::update(bool gameOver){
	theSun.tooFar();
	theSun.move();
		
	if (levelMove && blocksTemp[numBlocks-1].posx()==0){	//level has moved on screen such that the last block of the prior level is now the first block of the next
		levelMove=false;
		nextSet();
		numBlocks=numBlocksNext;		
	}
	else if (levelMove){
		next();
	}

	Mario.update();
				
	for (int i=0;i<numBlocks;i++){
		if (!gameOver && Mario.hit(blocks[i].posx(),blocks[i].posy(), 50, 50)){	//basic collision statement for not-Mario and every block on screen
			Mario.reposition(blocks[i].posx(), blocks[i].posy());
		}
		bBill.bulletCollide(blocks[i].posx(),blocks[i].posy(),50,50);	//basic collision statement for bullet and every block on screen
	}
	if (!gameOver && Mario.hit(bBill.posx(true), bBill.posy(true),50,50)){	//Bill is treated as a block
		Mario.reposition(bBill.posx(true), bBill.posy(true));
	}

	bBill.FIRE(Mario.posx(), difficulty);
	if (Mario.hit(coins.posx()+20, coins.posy()+17, 10, 15)){
		coins.init(-100,-100);
		Mario.itemPickup(0);
	}
	if (Mario.hit(flowers.posx()+18, flowers.posy()+27, 14, 23)){
		flowers.init(-100,-100);
		Mario.itemPickup(1);
	}
	if (Mario.hit(mushrooms.posx()+17, mushrooms.posy()+30, 14, 23)){
		mushrooms.init(-100,-100);
		Mario.itemPickup(2);
	} 
	if (!Mario.invulnerable()){	//enemy collision statements, ignored if not-Mario is invulnerable
		if (Mario.hit(spikes.posx()+5, spikes.posy()+40, 40, 10)){
			Mario.isHurt();
		}
		//initial: +10, +2, 20, 11
		if (Mario.hit(bBill.posx(false)+15, bBill.posy(false)+7, 15, 6)){
			Mario.isHurt();
		}
	}
	if (Mario.posx() > 748){	//determines if not-Mario has reached the end of the screen
		levelMove=true;
		nextCreate();
	}
	return levelMove;
}
