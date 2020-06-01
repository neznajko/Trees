#include <complex>
#define _USE_MATH_DEFINES // M_PI
#include <math.h>
#include <iostream>
#include <list>
#include <numeric> // accumulate
#include <iterator>
using namespace std;
////////________////////________////////________////////________
typedef double               real_t;
typedef complex<real_t>   complex_t;
typedef list<class Point>    dots_t;
typedef dots_t::iterator     pntr_t;
////////////////////////////////////////////////////////////////
////////````````////////````````////////````````////////--------
real_t rtod(const real_t r) { return r * 180 / M_PI; }
real_t dtor(const real_t d) { return d * M_PI / 180; }
////////\\\\\\\\////////\\\\\\\\////////\\\\\\\\////////________
class Point: public complex_t {
public:
	char lab; // label
	Point(char lab = 'Z', real_t x = 0, real_t y = 0): 
	 	complex_t(x, y), lab(lab) {}
	Point(const complex_t& z): complex_t(z) {} // +, -, ...
	real_t phi(void) const {
		const real_t phi = rtod(arg(*this));
		
		return (phi < 0)? phi + 360: phi;
	}
	real_t rho(void) const { return abs(*this);	}
	friend
	ostream& operator << (ostream& os, const Point& that) {
		return os << "(" << that.lab 
				  << ": " << that.real()
				  << ", " << that.imag()
				  << ", rho: " << that.rho()
				  << ", phi: " << that.phi()
				  << ")";
	}
	bool operator < (const Point& that) const { // sorting
    	return phi() < that.phi(); 
    }
	/* Return the absolute difference between <this/> and      *
     * <that/>'s <phi/> angles (less or equal 180 degrees).    *
	 *//////////////////////////////////////////////////////////
	real_t dlta_phi(const Point& that) {
		const real_t d = abs(phi() - that.phi());

		return (d > 180)? 360 - d: d;
	}
	/* Return the distance betveen <this/> and <that/> point.  *
	 *//////////////////////////////////////////////////////////
	real_t distance(const Point& that) {
		const real_t x = real() - that.real();
		const real_t y = imag() - that.imag();

		return hypot(x, y);
	}
}; /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////********////////********////////********////////________
// Let's practice some Design Patterns: (Singleton)
class Glob {
private:
	static Glob* instance; // uniQue
	Glob(void) {
		int n; // number of trees
		cin >> n;
		for (int j = n; j > 0; --j) {
			char lab;
			real_t x, y;
			cin >> lab >> x >> y;
			trees.push_back(Point(lab, x, y));
		}
		/* House points ////////////////////////////////////////
		//                                                    //
		//   ^                                                //
		//   ;       X         W                              //
		// d ;-----+---------/                                //
		//   ;     : U       : V                              //
		// b ;-----x---------*                                //
		//   +-----;---------}---->                           //
		//         a         c                                //
		//                                                    //
		//////////////////////////////////////////////////////*/
		real_t a, b, c, d;
		cin >> a >> b >> c >> d;
		house.push_back(Point('U', a, b));
		house.push_back(Point('V', c, b));
		house.push_back(Point('W', c, d));
		house.push_back(Point('X', a, d));
	}
public:
	static Glob* get_instance(void) {
		return (instance == nullptr)? new Glob: instance;
	}
	dots_t trees;
	dots_t house;
}* Glob::instance = nullptr;
////////////////////////////////////////////////////////////////
////////""""""""////////""""""""////////""""""""////////********
////////--------////////--------////////--------////////--------
// ////////////////////////////////////////////////////////// //
Point cm(const dots_t& t) {
	Point p = accumulate(t.begin(), t.end(), Point());
	p /= t.size();
	p.lab = 'Y';
	return p;
} /*////;;;;;;;;////////;;;;;;;;////////;;;;;;;;////////++++++++
*            C  <(CAB) = alpha ; A is the center of mass point *
*          .`   <(NAB) = phi   ; on the line AN there is a     *
*     b  .` `   <(ABC) = beta  ; tree point, we want to        *
*      .`   `   AC = b         ; determine whether it's inside *
*    .`     `   AB = c         ; ABC or outside, B and C are   *
*  .`. . . .` N AN = n         ; neigbor tree with respect to  *
* A `.   n  `                  ; phi in com. This function     *
*     `.    `   ;;;;;;;;;;;;;;;; will return the value of n so *
*     c `.  `   ; we can compare it with tree's distance.      *
*         `.`   ; Angles are in degrees.                       *
*           `B  ;                                              *
*///////////////////////////////////////////////////////////////
real_t n(const real_t& c, //                                  oo
		 const real_t& b,
		 real_t phi,
		 real_t alpha) {
	phi = dtor(phi);
	alpha = dtor(alpha); // not zero
	// cotangent of beta
	const real_t cot = abs( (b*cos(alpha) - c)/(b*sin(alpha)) );
	return c/(cos(phi) + cot*sin(phi));
} //////////////////////////////////////////////////////////////
class Forest: public dots_t {//                                _
private://                                                     _
 	Point cmass;// FeHTtp t8 Mac                               _
public://                                                      _
 	Forest(const dots_t& t): dots_t(t) {// copy cons           _
 		cmass = cm(*this);//                                   _
 	}//                                                        _
	void com() {// Center Of Mass Frame                        _
		for (auto& p: *this) { p -= cmass; }//                 _
		sort();//                                              _
	}//                                                        _
	friend//                                                   _
	ostream& operator << (ostream& os, const Forest& f)	{//    _
		copy(f.begin(), f.end(), ostream_iterator<Point>(os, "\n"));//_
		//                                                     _
		return os;//                                           _
	}//                                                        _
	pntr_t next(pntr_t p) {// How to figure that with          _
		if (++p == end()) {// iterators?, vhich are            _
			return begin();// deprecated??                     _
		}//                                                    _
		return p;//                                            _
	}//                                                        _
	pntr_t prev(pntr_t p) {// MacTtp t8 ntnuT                  _
		if (p == begin()) {//                                  _
			p = end();//                                       _
		}//                                                    _
		return --p;//                                          _
	}//                                                        _
	bool visible(const pntr_t& N) { // FeHTtp 2	               _
		const pntr_t C = next(N);// counter-clock-wise-comment _
		const pntr_t B = prev(N);//                            _
		const real_t phi   = N->dlta_phi(*B);//                _
		const real_t alpha = N->dlta_phi(*C) + phi;//          _
		//                                                     _
		if (alpha >= 180) return true;//                       _
		//                                                     _
		const real_t c = B->rho();//                           _
		const real_t b = C->rho();//                           _
		//                                                     _
		if (N->rho() < n(c, b, phi, alpha)) return false;//    _
		//                                                     _
		return true;//                                         _
	}//                                                        _
	/* By temporarily inserting a house point <h> ck vhether   *
	 * it's inside parameter defined by the forest <t>.        *
	 *//////////////////////////////////////////////////////////
	bool ckck(Point h /* copy */) {/**//////////////////////////
		/* com *////////////////////////////////////////////////
		h -= cmass;/**//////////////////////////////////////////
		/* find insert position *///////////////////////////////
		pntr_t it;/**///////////////////////////////////////////
		for (it = begin(); it != end() and *it < h; ++it)/**////
			;/**////////////////////////////////////////////////
		/* insert <h> just before <it> and update */////////////
		it = insert(it, h);/**//////////////////////////////////
		/* ck if visible *//////////////////////////////////////
		bool ck = visible(it);/**///////////////////////////////
		/* erase <h> *//////////////////////////////////////////
		erase(it);/**///////////////////////////////////////////
		/**/////////////////////////////////////////////////////
		return ck;/**///////////////////////////////////////////
	}/**////////////////////////////////////////////////////////
	/* loof and remove not visible trees until don! *///////////
	void rewind() {/**//////////////////////////////////////////
		for (;;) {/**///////////////////////////////////////////
			size_t siz = size();/* size *///////////////////////
			/* hardcore loof *//////////////////////////////////
			for (pntr_t p = begin(); p != end();) {/**//////////
				if (!visible(p)) {/**///////////////////////////
					cout << "Erasing: " << *p << "\n";/**///////
					erase(p++);/**//////////////////////////////
					continue;/**////////////////////////////////
				}/**////////////////////////////////////////////
				++p;/**/////////////////////////////////////////
			}/**////////////////////////////////////////////////
			if (siz == size()) break;/* ve ar don! *////////////
		}/**////////////////////////////////////////////////////
	}/**////////////////////////////////////////////////////////
	real_t par() {/**///////////////////////////////////////////
		/* Calculate parameter .. */////////////////////////////
		real_t par = 0;/**//////////////////////////////////////
		for (pntr_t p = begin(); p != end(); ++p) {/**//////////
			par += p->distance(*next(p));/**////////////////////
		}/**////////////////////////////////////////////////////
		return par;/**//////////////////////////////////////////
	}/**////////////////////////////////////////////////////////
	const Point& get_cmass() const { return cmass; } ///////////
}; /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
int main(void) {//_
	// TST:_
	Glob* glob = Glob::get_instance(); // redirect infut_
 	//_
 	Forest f(glob->trees);//_
 	//_
 	cout << "Forest:\n" << f;//_
 	f.com();// move into center of mass_
 	cout << "COM: " << f.get_cmass() << "\n"//_
		 << f << "Rewinding ..\n";//_
 	f.rewind();//_
 	cout << "Forest Guards:\n" << f//_
 		 << "Parameter: " << f.par() << "\nHouse Coors:\n";//_
 	// ck house placement_
 	const dots_t& home = glob->house;//_
 	for (const auto& h: home) {//_
 		cout << h << " " << f.ckck(h) << "\n";//_
 	}//_
 	cout.flush();//_
	//_
	return 0;//_
}//_
////////////////////////////////////////////////////////////////
////////========////////========////////========////////=== log:
// gBe Hema Ha KoHFepTa me uMa nptkKu c 7yK, HoceTe cu cTo74eTa!
