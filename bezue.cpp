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


//место - номер в массиве, количество - колво значимых элементов массива
void ins_point(k_point * p,k_point & chto, int mesto, int & kolvo){
	if (kolvo==MAXPOINTS) throw ex(40);
	if (mesto > kolvo-1) throw ex(40); //throw
	for (int i=kolvo-1;i>=mesto;i--) p[i+1]=p[i];//сдвиг	
	p[mesto]=chto;
	kolvo++;
}
//количество передается по ссылке, это важно
void del_point(k_point * p, int mesto, int & kolvo){
	if (kolvo<1) return ; //thrwo
	if (mesto>kolvo-1) throw ex(41); //throw
	for (int i=mesto;i<kolvo-1;i++){//сдвиг
		p[i]=p[i+1];
	}
	kolvo--;
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

k_point polinom(k_point * p,int amount,double t){
	k_point buf;
	for (int i=0;i<amount;i++){
		buf=buf+p[i]*b(amount-1,i,t);
	}
	return buf;
}

k_point de_castel(double t, int i, int k, k_point * p){
	k_point buf;
	if (k==0) return p[i];
	else
	return de_castel(t,i,k-1,p)*(1-t)+de_castel(t,i+1,k-1,p)*t;
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

int check_points(k_point * p,int amount, k_point what){
	for (int i=0;i<amount;i++){
		if (abs(p[i].x-what.x)<=8 && abs(p[i].y-what.y)<=8) return i;
	}
	return -1;
}

void draw_points(k_point * p,int amount,RenderWindow & window,sf::Color clr){
	CircleShape shape(2,100);
	shape.setFillColor(clr);
	for (int i=0;i<amount;i++){
		shape.setPosition(p[i].x-2,p[i].y-2);
		window.draw(shape);
	}
}

void draw_line(k_point p1,k_point p2,RenderWindow & window, sf::Color clr){
	sf::Vertex pts[2];
	pts[0].position = sf::Vector2f(p1.x,p1.y);
	pts[1].position = sf::Vector2f(p2.x,p2.y);
	pts[0].color=clr;
	pts[1].color=clr;
	window.draw(pts, 2, sf::Lines);
}

void draw_curve(k_point * p, int amount, RenderWindow & window,sf::Color clr,int mode){
	if (mode!=0 && mode !=1) return;
	if (amount==0) return ;
	k_point buf1,buf2=p[0];
	double t=0;
	double step=1/((double)amount*STEP);
	if (mode==1) for (int i=0;i<amount-1;i++) draw_line(p[i],p[i+1],window,Color::Green);
	while (1){
		buf1=polinom(p,amount,t);
		//buf1=de_castel(t,0,amount-1,p);//one or another choose, de castel too slow
		draw_line(buf1,buf2,window,clr);
		buf2=buf1;
		t=t+step;
		if (t>=1) {
			t=1;
			buf1=polinom(p,amount,t);
			draw_line(buf1,buf2,window,clr);
			break;
		}
	}
}

void dnd(k_point * p,int amount,int mode, RenderWindow & window,Color clr){
	window.clear(Color::White);
	draw_curve(p,amount,window,clr,mode);
	if (mode) draw_points(p,amount,window,clr);
	window.display();
}

void get_cursor(k_point * p){
	int i=0;
	int mode=1;
	RenderWindow window(VideoMode(X,Y),"click points");
	window.clear(Color::White);
	window.display();
	while (window.isOpen()){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();//закрытие
			else
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
				mode = mode ^1;
				dnd(p,i,mode,window,Color::Black);
			}
			else
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::D){
				del_point(p,i-1,i);
				dnd(p,i,mode,window,Color::Black);;
			}
			else
			if (event.type ==sf::Event:: MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left ){ //кнопка нажата
				if (i==MAXPOINTS) throw ex(3);//throw
				int num;
				k_point buf;
				buf.x=event.mouseButton.x;
				buf.y=event.mouseButton.y;
				num=check_points(p,i,buf);
				if (num==-1) {
					p[i].x=event.mouseButton.x;
					p[i].y=event.mouseButton.y;
					i++;
					window.clear(Color::White);
					draw_curve(p,i,window,Color::Black,mode);
					if (mode==1) draw_points(p,i,window,Color::Black);
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
								draw_curve(p,i,window,Color::Black,mode);
								if (mode) draw_points(p,i,window,Color::Black);
								if (mode) draw_points(&p[num],1,window,Color::Red);
								window.display();
							} 
							else 
							if (event0.type == sf::Event:: MouseButtonReleased && event0.mouseButton.button == sf::Mouse::Left){//кнопка отжата
								p[num].x=check_border(event0.mouseButton.x,X,BORDER);
								p[num].y=check_border(event0.mouseButton.y,Y,BORDER);
								dnd(p,i,mode,window,Color::Black);
								turn=1;
							}
							else
							if (event0.type == Event::KeyPressed && event0.key.code == Keyboard::A){
								ins_point(p,p[num],num,i);
								num++;
							}
							else
							if (event0.type == Event:: KeyPressed && event0.key.code == Keyboard::D){
								turn=1;
								del_point(p,num,i);
								dnd(p,i,mode,window,Color::Black);
							}
						}
					}
				}
			}
		}
	}
}


int main(){
	try{
		k_point p[MAXPOINTS];
		get_cursor(p);
	}
	catch(ex a){a.out();}
	catch(...){printf("what?\n");}
	return 0;
}
