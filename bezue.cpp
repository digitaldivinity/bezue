#include <stdio.h>
#include <SFML/Graphics.hpp>

#define X 800
#define Y 800
#define BORDER 10
#define STEP 10
#define MAXPOINTS 100

using namespace sf;

struct k_point{
	double x,y;
	k_point(){
		x=y=0;
	}
	k_point(const double & x, const double & y){
		this->x=x;
		this->y=y;
	}
	k_point operator*(double num){
		k_point ob(x*num,y*num);
		return ob;
	}
	k_point operator+(k_point ob){
		k_point r(x+ob.x,y+ob.y);
		return r;
	}
	k_point operator/(double ob){
		k_point buf(x/ob,y/ob);
		return buf;
	}
	k_point operator=(k_point ob){
		this->x=ob.x;
		this->y=ob.y;
		return *this;
	}
	void out(){
		printf("%lf %lf\n",x,y);
	}
};

class ex{
	int a;
	public:
	ex(int i=0) : a(i) {}
	void out(){
		switch (a){
			
			case 1: printf("one\n"); break;
			case 2: printf("two\n"); break;
			case 3: printf("array overflow error\n"); break;
			case 5: printf("Неполный набор параметров\n"); break;
			case 40: printf("Insert point error\n"); break;
			case 41: printf("Delete point error\n"); break;
			default: printf("undef error\n"); break;
		}
	}
};
//глобальные переменные сократят описание функций
k_point p[MAXPOINTS];
double w[MAXPOINTS];
int mode=1;
int amount=0;
RenderWindow window(VideoMode(X,Y),"click points");
sf::Text text;
sf::Font font;
char str[32];


void ins_point(k_point & chto, int mesto){
	if (amount==MAXPOINTS) throw ex(40);
	if (mesto > amount-1) throw ex(40); //throw
	for (int i=amount-1;i>=mesto;i--) p[i+1]=p[i];//сдвиг	
	p[mesto]=chto;
	amount++;
}

void del_point(int mesto){
	if (amount<1) return ; //thrwo
	if (mesto>amount-1) throw ex(41); //throw
	for (int i=mesto;i<amount-1;i++){//сдвиг
		p[i]=p[i+1];
	}
	amount--;
}

double pow(const double & x,const int & y){
	int i;
	double buf=x;
	if (y==0) return 1;
	if (y<0) throw ex(15); //throw
	for (i=0;i<y-1;i++){
		buf*=x;
	}
	return buf;
}

double f(const int & n){
	if (n<0) throw ex(20); //throw
	double k=1;
	for (int i=1;i<=n;i++) k*=i;
	return k;
}

double c(const int & n, const int & i){
	return (f(n)/(f(i)*f(n-i)));
}

double b(const int & n, const int & i, const double & t){
	return c(n,i)*pow(t,i)*pow((1-t),n-i);
}

k_point polinom(double t){
	k_point buf;
	double s;
	double sum=0;
	for (int i=0;i<amount;i++){
		s=w[i]*b(amount-1,i,t);
		buf=buf+p[i]*s;
		sum+=s;		
	}
	/*
	for (int i=0;i<amount;i++){
		buf=buf+p[i]*b(amount-1,i,t);
	}
	*/
	return buf/sum;
}

k_point de_castel(double t, int i, int k){
	k_point buf;
	if (k==0) return p[i];
	else
	return de_castel(t,i,k-1)*(1-t)+de_castel(t,i+1,k-1)*t;
}

double abs(double x){
	if (x<0) return -x;
	else if (x==0) return 0;
	else return x;
}

double check_border(double x,double window,double border){
	if (x<border) return border;
	else if (x>window) return window-border;
	else return x;
}

int check_points(k_point what){
	for (int i=0;i<amount;i++){
		if (abs(p[i].x-what.x)<=8 && abs(p[i].y-what.y)<=8) return i;
	}
	return -1;
}

//здесь есть возможность отрисовывать некоторые точки из всего массива
void draw_points(k_point * p,int amount,sf::Color clr){
	CircleShape shape(2,100);
	shape.setFillColor(clr);
	for (int i=0;i<amount;i++){
		shape.setPosition(p[i].x-2,p[i].y-2);
		window.draw(shape);
	}
}

void draw_line(k_point p1,k_point p2, sf::Color clr){
	sf::Vertex pts[2];
	pts[0].position = sf::Vector2f(p1.x,p1.y);
	pts[1].position = sf::Vector2f(p2.x,p2.y);
	pts[0].color=clr;
	pts[1].color=clr;
	window.draw(pts, 2, sf::Lines);
}

void draw_curve(sf::Color clr){
	if (mode!=0 && mode !=1) return;
	if (amount==0) return ;
	k_point buf1,buf2=p[0];
	double t=0;
	double step=1/((double)amount*STEP);
	if (mode==1) for (int i=0;i<amount-1;i++) draw_line(p[i],p[i+1],Color::Green);
	while (1){
		buf1=polinom(t);
		//buf1=de_castel(t,0,amount-1,p);//one or another choose, de castel too slow
		draw_line(buf1,buf2,clr);
		buf2=buf1;
		t=t+step;
		if (t>=1) {
			t=1;
			buf1=polinom(t);
			draw_line(buf1,buf2,clr);
			break;
		}
	}
}

void dnd(sf::Color clr){
	window.clear(Color::White);
	draw_curve(clr);
	if (mode) draw_points(p,amount,clr);
	window.display();
}

void dnd2(){
	window.clear(Color::White);
	draw_curve(sf::Color::Black);
	if (mode) draw_points(p,amount,sf::Color::Black);
	text.setString(str);
	window.draw(text);
	window.display();
}


int main(){
	font.loadFromFile("Gascogne.ttf");
	text.setFont(font);
	text.setCharacterSize(20);
	text.setColor(sf::Color::Black);
	k_point buf;
	
	int num;
	int i;
	for (i=0;i<MAXPOINTS;i++) w[i]=1;
	i=0;
	window.clear(Color::White);
	window.display();
	while (window.isOpen()){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();//закрытие
			else
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
				mode = mode ^1;
				dnd(Color::Black);
			}
			else
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::D){
				del_point(amount-1);
				dnd(Color::Black);;
			}
			else if (event.type == sf::Event::MouseWheelMoved){
				buf.x=event.mouseWheel.x;
				buf.y=event.mouseWheel.y;
				if (-1!=(num=check_points(buf))){
					if (w[num]>0.7 || event.mouseWheel.delta>0) w[num]+=event.mouseWheel.delta/2.0;
					text.setPosition(p[num].x-20,p[num].y-20);
					sprintf(str,"%2.1lf",w[num]);
					dnd2();
				}
			}
			else
			if (event.type ==sf::Event:: MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left ){ //кнопка нажата
				if (amount==MAXPOINTS-1) throw ex(3);//throw
				buf.x=event.mouseButton.x;
				buf.y=event.mouseButton.y;
				num=check_points(buf);
				if (-1==(num=check_points(buf))) {
					p[amount].x=event.mouseButton.x;
					p[amount].y=event.mouseButton.y;
					amount++;
					window.clear(Color::White);
					draw_curve(Color::Black);
					if (mode==1) draw_points(p,amount,Color::Black);
					window.display();
				}
				else
				{
					int turn = 0;
					while (!turn){
						Event event0;
						while (window.pollEvent(event0)){
							if (event0.type ==sf::Event::MouseMoved){//мышка переместилась с зажатой кнопкой
								p[num].x=check_border(event0.mouseMove.x,X,BORDER);
								p[num].y=check_border(event0.mouseMove.y,Y,BORDER);
								window.clear(Color::White);
								draw_curve(Color::Black);
								if (mode) draw_points(p,amount,Color::Black);
								if (mode) draw_points(&p[num],1,Color::Red);
								window.display();
							} 
							else 
							if (event0.type == sf::Event:: MouseButtonReleased && event0.mouseButton.button == sf::Mouse::Left){//кнопка отжата
								p[num].x=check_border(event0.mouseButton.x,X,BORDER);
								p[num].y=check_border(event0.mouseButton.y,Y,BORDER);
								dnd(Color::Black);
								turn=1;
							}
							else
							if (event0.type == Event::KeyPressed && event0.key.code == Keyboard::A){
								ins_point(p[num],num);
								num++;
							}
							else
							if (event0.type == Event:: KeyPressed && event0.key.code == Keyboard::D){
								turn=1;
								del_point(num);
								dnd(Color::Black);
							}
						}
					}
				}
			}
		}
	}
}
