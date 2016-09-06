/*
 * Screen.h
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#ifndef SCREEN_H_
#define SCREEN_H_

class Screen{
private:
	int lowu, lowv, highu, highv;
public:
	Screen(){lowu = 0, lowv = 0; highu = 0; highv = 0;};
	Screen(int, int, int, int);
	Screen(const Screen &other){
		lowu = other.getlowu();
		lowv = other.getlowv();
		highu = other.gethighu();
		highv = other.gethighv();
	}
	int getlowu() const { return lowu; }
	int getlowv() const { return lowv; }
	int gethighu() const { return highu; }
	int gethighv() const { return highv; }
};

#endif /* SCREEN_H_ */
