/******************************************************************************
 *
 * $Id$
 *
 * Project:  WindNinja
 * Purpose:  Class for storing a 3D array
 * Author:   Jason Forthofer <jforthofer@gmail.com>
 *
 ******************************************************************************
 *
 * THIS SOFTWARE WAS DEVELOPED AT THE ROCKY MOUNTAIN RESEARCH STATION (RMRS)
 * MISSOULA FIRE SCIENCES LABORATORY BY EMPLOYEES OF THE FEDERAL GOVERNMENT 
 * IN THE COURSE OF THEIR OFFICIAL DUTIES. PURSUANT TO TITLE 17 SECTION 105 
 * OF THE UNITED STATES CODE, THIS SOFTWARE IS NOT SUBJECT TO COPYRIGHT 
 * PROTECTION AND IS IN THE PUBLIC DOMAIN. RMRS MISSOULA FIRE SCIENCES 
 * LABORATORY ASSUMES NO RESPONSIBILITY WHATSOEVER FOR ITS USE BY OTHER 
 * PARTIES,  AND MAKES NO GUARANTEES, EXPRESSED OR IMPLIED, ABOUT ITS QUALITY, 
 * RELIABILITY, OR ANY OTHER CHARACTERISTIC.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/

#ifndef WN_3D_ARRAY_H
#define WN_3D_ARRAY_H

#include "ninjaException.h"

class wn_3dArray
{
	public:
		wn_3dArray();								//Default constructor
		wn_3dArray(int rows, int cols, int layers);	//Constructor
		~wn_3dArray();                              // Destructor
		
		wn_3dArray(wn_3dArray const& m);               // Copy constructor
		wn_3dArray& operator= (wn_3dArray const& m);   // Assignment operator

		void allocate(int rows, int cols, int layers);	//make 3d array of this size, re-allocate if necessary
		void deallocate();			//kills memory (data_ array)

                double getMaxValue();
                double getMinValue();
		
		double& operator() (int row, int col, int layer);
		double  operator() (int row, int col, int layer) const;
		double& operator() (int num);
		double  operator() (int num) const;
		
		int rows_, cols_, layers_;

	private:
		
		double* data_;
};

#endif /* WN_3D_ARRAY_H */
