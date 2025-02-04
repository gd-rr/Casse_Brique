// apps/LIFAPI/LIFAPI_TP.cpp
#include<Grapic.h>
#include<math.h>

using namespace grapic;

const int DIMW=500;
const float dt=0.01;
const float Friction = 0.9;
const int MAX=100;

struct Vec2{
    float x;
    float y;
};

Vec2 make_Vec2 (float x, float y)
{
    Vec2 c;
    c.x = x;
    c.y = y;
    return c;
}
Vec2 operator+ (Vec2 z, float l)
{
    return make_Vec2 (z.x +l, z.y);
}

Vec2 operator+ (Vec2 a, Vec2 b)
{
    return make_Vec2 (a.x + b.x,a.y + b.y);
}

Vec2 operator- (Vec2 a, Vec2 b)
{
    return make_Vec2 (a.x - b.x , a.y - b.y);
}

Vec2 operator* (Vec2 a, float l)
{
    Vec2 c;
    c.x = a.x *l;
    c.y = a.y * l;
    return c;
}

Vec2 operator* (float l, Vec2 a)
{
    Vec2 c;
    c.x = l*a.x;
    c.y = l*a.y;
    return c;
}

Vec2 operator* (Vec2 a, Vec2 b)
{
    return make_Vec2 (a.x * b.x - a.y * b.y , a.y * b.x + a.x * b.y);
}

struct Color{
    unsigned char r, g, b;
};

Color make_color(unsigned char r, unsigned char g, unsigned char b){
    Color c;
    c.r=r;
    c.g=g;
    c.b=b;
    return c;
}

Color operator+(Color A, Color B){
    return make_color(A.r+B.r, A.g+B.g, A.b+B.b);
}

Color operator*(Color A, float l){
    return make_color(A.r*l, A.g*l,A.b*l);
}

Color operator*(float l, Color A){
    return make_color(l*A.r, l*A.g,l*A.b);
}

struct Particle{
    Vec2 p;
    Vec2 v;
    Vec2 vmax;
    Vec2 f;
    float g;
    float m;
};

struct Brique{
    int etat;
    Color c;
    Vec2 pmin;
    Vec2 pmax;
};

struct Wall{
    Brique grille[MAX][MAX];
    int dx, dy;
    Image fond;
};

struct Barre{
    int xmin;
    int ymin;
    int xmax;
    int ymax;
};

void InitPart (Particle &part){
    part.p=make_Vec2(DIMW/2, 195);
    part.v=make_Vec2(0, 0);
    part.vmax = make_Vec2(100, 100);
    part.f=make_Vec2(0, 0);
    part.g=-9.81;
    part.m=1.0;
}

void InitWall(Wall &w){
    int i, j;
    w.fond = image ("data/fond casse brique.jpg");
    w.dx = 10;
    w.dy = 10;
    for (i=0; i<w.dx; i++){
        for (j=0; j<w.dy; j++){
            w.grille[i][j].etat = 3;
            w.grille[i][j].pmin = make_Vec2(i * (DIMW / w.dx), j * ((DIMW - 200) / w.dy) +200);
            w.grille[i][j].pmax = make_Vec2((i+1) * (DIMW / w.dx), (j+1) * ((DIMW - 200) / w.dy)+200);
            w.grille[i][j].c.r = 85;
            w.grille[i][j].c.g = 0;
            w.grille[i][j].c.b = 90;
        }
    }
}

void InitBarre (Barre &b){
    b.xmin = DIMW/2 - 25;
    b.xmax = DIMW/2 + 25;
    b.ymin = 70;
    b.ymax = 75;
}

void DrawPart (Particle part){
    color(255,255,255);
    circleFill(part.p.x, part.p.y, 3);
}

void DrawWall(Wall& w, Particle& part){
    int i, j;
    image_draw(w.fond, 0, 0, DIMW - 1 ,DIMW - 1);
    for (i=0; i<w.dx; i++){
        for (j=0; j<w.dy; j++){
            if (w.grille[i][j].etat > 0){
                color(w.grille[i][j].c.r, w.grille[i][j].c.g, w.grille[i][j].c.b);
                rectangleFill( w.grille[i][j].pmin.x, w.grille[i][j].pmin.y, w.grille[i][j].pmax.x, w.grille[i][j].pmax.y);
                color(0,0,0);
                rectangle(w.grille[i][j].pmin.x, w.grille[i][j].pmin.y, w.grille[i][j].pmax.x, w.grille[i][j].pmax.y);
            }
        }
    }
}

void DrawBarre (Barre b){
    color (255, 255, 255);
    rectangleFill(b.xmin, b.ymin, b.xmax, b.ymax);
}

void addParticleForce (Particle &part, Vec2 F){
    part.f=part.f+F;
}

/* Une amélioration possible : utliser un SDF pour  les collisions briques/bille
Vec2 MaxVec (Vec2 a, Vec2 b){
    Vec2 r;
    r.x = std::max(a.x, b.x);
    r.y = std::max(a.y, b.y);
    return r;
}

float Length (Vec2 a){
        return sqrt((a.x * a.x) + (a.y * a.y)) ;
}

float SDF (Vec2 p, Vec2 pmin, Vec2 pmax){
    Vec2 ppmin = pmin - p;
    Vec2 pmaxp = p - pmax;
    Vec2 d = make_Vec2(std::max(ppmin.x, pmaxp.y), std::max(ppmin.y, pmaxp.y));
    Vec2 nul =  make_Vec2(0, 0);
    float l = Length(MaxVec(nul, d));
    return l;
}
*/

void CollisionWall (Particle &part, Wall &w){
    int i, j;
    float d;
    if(part.p.y >= DIMW){ // haut fenetre
        part.p.y = DIMW-(part.p.y-DIMW);
        part.v.y = -part.v.y;
    }
    if(part.p.x <= 0){ // gauche fenetre
        part.p.x = -part.p.x;
        part.v.x = -part.v.x;
    }
    if(part.p.x >= DIMW){ // droite fenetre
        part.p.x = DIMW-(part.p.x-DIMW);
        part.v.x = -part.v.x;
    }

    for (i=0; i<w.dx; i++){
        for (j=0; j<w.dy; j++){
            if(w.grille[i][j].etat > 0) {
                //d = SDF(part.p, w.grille[i][j].pmin, w.grille[i][j].pmax);
                Vec2 pmin = w.grille[i][j].pmin;
                Vec2 pmax = w.grille[i][j].pmax;
                Vec2 p = part.p;
                bool col = p.x > pmin.x && p.x < pmax.x && p.y > pmin.y && p.y < pmax.y;
                if (col){
                    if(part.p.y >= w.grille[i][j].pmin.y){ //bas brique
                        part.v.y = -part.v.y;
                    }
                    else if(part.p.y <= w.grille[i][j].pmax.y){ //haut brique
                        part.v.y = -part.v.y;
                    }
                    else if(part.p.x >= w.grille[i][j].pmin.x){ //droite brique
                        part.v.x = -part.v.x;
                    }
                    else if(part.p.x <= w.grille[i][j].pmax.x){ //gauche brique
                        part.v.x = -part.v.x;
                    }
                    part.v = part.v * Friction;
                    w.grille[i][j].etat = w.grille[i][j].etat - 1;
                }

            }
        }
    }

}

void UpdatePartWall (Particle &part, Wall &w){
    int i, j;
    Vec2 F;
    part.f=make_Vec2(0,0);
    F=make_Vec2(0, part.g*part.m);
    addParticleForce(part, F);
    part.v = part.v + dt * (1. / part.m * part.f);
    part.p=part.p+part.v*dt;
    for (i=0; i<w.dx; i++){
        for (j=0; j<w.dy; j++){
            if (w.grille[i][j].etat == 2){
                w.grille[i][j].c.r = 140;
                w.grille[i][j].c.g = 60;
                w.grille[i][j].c.b = 150;
            }
            if (w.grille[i][j].etat == 1){
                w.grille[i][j].c.r = 224;
                w.grille[i][j].c.g = 0;
                w.grille[i][j].c.b = 255;
            }
        }
    }
    CollisionWall (part, w);
}

void UpdateBarrePart (Barre &b, Particle &p){
    if (isKeyPressed(SDLK_LEFT)){
        b.xmin = b.xmin - 10;
        b.xmax = b.xmax - 10;
    }
    if (isKeyPressed(SDLK_RIGHT)){
        b.xmin = b.xmin + 10;
        b.xmax = b.xmax + 10;
    }
    bool col = p.p.x > b.xmin && p.p.x < b.xmax && p.p.y > b.ymin && p.p.y < b.ymax;
    if (col){
        if(p.p.y >= b.ymin){
            p.v.y = -p.v.y;
        }
        else if(p.p.y <= b.ymax){
            p.v.y = -p.v.y;
        }
        else if(p.p.x >= b.xmin){
            p.v.x = -p.v.x;
        }
        else if(p.p.x <= b.xmax){
            p.v.x = -p.v.x;
        }
        p.v = p.v * Friction ;
        if (p.v.y < p.vmax.y){
            p.v.y = p.v.y + 10;
        }
        p.v=make_Vec2(frand(-10,10), p.v.y);
    }
}

int main(int , char**)
{
    Particle p;
    Wall w;
    Barre b;

    bool stop=false;
	winInit("Casse Briques de l'espace",DIMW,DIMW);
	backgroundColor(255,255,255);

	InitPart(p);
	InitWall(w);
    InitBarre(b);

	while( !stop )
    {
        winClear();

        DrawWall(w, p);
        DrawPart(p);
        DrawBarre(b);
        UpdatePartWall(p, w);
        UpdateBarrePart(b,p);
        delay(2);

        stop = winDisplay();
    }
	pressSpace();
	winQuit();
	return 0;
}
