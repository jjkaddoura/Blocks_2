void display();
void drawCube(GLfloat side);
void drawSolidCube(GLfloat side, float r, float g, float b);
void drawTextureCube(GLfloat side);
void reshape(int width, int height);
void drawCubeEdges(float side, float r, float g, float b);
void drawAllNumbers();
void drawNumber(int number);
void drawLetter(char letter);
void drawZero();
void drawOne();
void drawTwo();
void drawThree();
void drawFour();
void drawFive();
void drawSix();
void drawSeven();
void drawEight();
void drawNine();
void drawA();
void drawB();
void drawC();
void drawD();
void drawRing(GLfloat angle, GLfloat depth, GLfloat oRadius, GLfloat iRadius);
void drawRingOutLine(GLfloat angle, GLfloat depth, GLfloat oRadius, GLfloat iRadius, GLfloat Rout, GLfloat     Gout, GLfloat Bout, GLfloat Rin, GLfloat Gin, GLfloat inB);
void drawSemiCircle(GLfloat angle, GLfloat oRadius, GLfloat iRadius);
void drawFlipSemiCircle(GLfloat angle, GLfloat oRadius, GLfloat iRadius);
void drawDepthOfCircle(GLfloat angle, GLfloat depth, GLfloat oRadius);
void drawNumOnCube(int side, int number);
void drawLetterOnCube(int side, char letter);
void initTexture();

void init();
													 
