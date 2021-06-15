/*  Copyright (C) 2021 @lyazj
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
