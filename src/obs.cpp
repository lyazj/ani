#include "Observer.h"

using namespace std;

int main()
{
  Observer observer;

  cout << "Where is the observer? (x y z) ";
  if(!(cin >> observer.r0))
    goto END;

  cout << "How do you place the coordinate? [degrees] (theta phi) ";
  if(!(cin >> observer.d0))
    goto END;
  for(double &d : observer.d0.data)
    d = radians(d);

  Vector<double, 3> r;
  while(true)
  {
    cout << "Where is the object you want to see? (x y z) ";
    if(!(cin >> r))
      break;
    cout << "In your coordinate, look for it at (x y z): "
         << observer.observe(r) << endl;
    cout << "Type \'end\' to exit. " << endl;
  }

END:
  cout << "Bye! " << endl;
  return 0;
}
