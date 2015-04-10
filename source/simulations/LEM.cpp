#include "LEM.h"
#include "OperationInterface.h"
#include "StatFile.h"

namespace SIMULATION 
{
	
LEM::LEM(ostream & os, ParamHandler & params): Simulation(os,params)
{}

bool LEM::run()
{
	/*
	LEMParams & params = static_cast<LEMParams &>(_params);
	setOutputDirectory(params.getOutputDirectory().c_str());
	setOutflowType(params.getOutflowType());
    */

   // output directory: save every output into this directory
	   //setOutputDirectory("d:\\terRAIN\\test");
	setOutputDirectory("d:\\pcrinst");
	   /*
	ofTopLeft ,
	ofTopRight,
	ofBottomLeft,
	ofBottomRight,
	ofTopSide,
	ofRightSide,
	ofBottomSide,
	ofLeftSide,
	ofAllSides,
	ofTopAndBottomSide,
	ofLeftAndRightSide
	*/
	//setOutflowType(ofBottomSide);
	//setOutflowType(ofTopAndBottomSide);
	setOutflowType(ofAllSides);
	//setOutflowType(ofBottomLeft);
	
	
	   // model size:
	   size_t nSizeX = 60;  //797;
	   size_t nSizeY = 60;  //788;
	   size_t slopeBreak = 1;		// can not be zero!
	   // statfile:
	   StatFile statFile("statfile",15);
	   StatFile statFile2("statfile2",15);
	   // in the accuflux function neglet the flow if account is less then 
	   // initial flow account*accuFluxLimitRate
	   double dx = 10;
	   double tangent = 0.0;
	   double kDiffDouble = 0e-3;
	   double kFluvDouble = 1e-5;
	   double kTectDouble = 1e-3;				// set below too!
	   double accuFluxLimitRate = 0.0;
	   double mxRandom2Max = 0;
	   double mxAccfluxMax = 0;
	   double mxLocalMin = 0;
	   //double limitUpstream = 0.01;
	   // decrease the boundary pixels in every iteration with this value:
	   //double boundDecrease = 0.01;
	   //double Kfluv = 0.1;
	   //double rain = 0.01;
	   double rand = 0.000001;
	   //double Susp = 0;
	   // 1 for TRL, 0 for DTL
	   double xCoordFP = 0;
	   double yCoordFP = 0;
	   //double zCoordFP = 0;
	   double yPositionFP = 0;
	   double zPositionFP = 0;
	   double yPositionFPOld = 0;
	   double zPositionFPOld = 0;
	   double focusPointMigration = 0;
	   double xCoordCircleMax = 0;
	   double yCoordCircleLimit = 0;
	   double yCoordCircleMin = (nSizeY-1)*dx;
	   double c1p1x = nSizeX*dx;	   
	   double c1p2x = 0;	   
	   double c1p3x = 0;	   
	   double c1p4x = 0;
	   double c1p5x = 0;	   
	   double c1p6x = 0;
	   double h1p1x = nSizeX*dx;	   
	   double h1p2x = 0;	   
	   double catchment1Width = 0;
	   double hollowWidth = 0;
	   double a = 1;
	   double a2 = 0.3;
	   double hollowShape = 0;
	   double catchmentShape = 0;
	   double depthFP = 0;
	   double widthRatio = 0;
	   double path1sum = 0;
	   double path2sum = 0;


	   // number of iterations:
	   size_t iterationNumber = 1000;
	   size_t i = 1;
       size_t j = 0;
	   size_t l = 0;
	   size_t lmax = 0;
       size_t jmax = 0;
	   size_t l2 = 0;
       size_t j2 = 0;
	   size_t pathsum = 0;
	   size_t areaCatchment3 = 0;
	   	   
       DblRasterMx mx0;
	   DblRasterMx mx1;
	   DblRasterMx mx3;
	   DblRasterMx tst2;
	   DblRasterMx tst3;
	   DblRasterMx one;
	   DblRasterMx minusone;
	   DblRasterMx zero;
	   DblRasterMx fifty;
	   DblRasterMx ten;
	   DblRasterMx five;
	   DblRasterMx half;
	   DblRasterMx zeroPlusABit;
	   DblRasterMx mxEdges;
       DblRasterMx mxRandom;
       DblRasterMx mxRandom2;
	   //DblRasterMx mxRandom2Max;
	   DblRasterMx mxSlope;
	   DblRasterMx mxSlopeCorrected;
       DblRasterMx mxAccflux;
	   DblRasterMx mxAccflux2;
	   DblRasterMx mxAccflux3;
       DblRasterMx mxFluid;
	   DblRasterMx change;
	   DblRasterMx mxUpstreamMin;
	   DblRasterMx lowerLimit;
	   DblRasterMx upperLimit;
	   DblRasterMx sedOut;
	   DblRasterMx sedOutWithoutDiff;
	   DblRasterMx sedIn;
	   //DblRasterMx DiffSedOut;
	   //DblRasterMx DiffSedIn;
	   DblRasterMx mxEroded;
	   DblRasterMx susp;
	   DblRasterMx boundDecrease;
	   DblRasterMx limitUpstream;
	   DblRasterMx m;
	   DblRasterMx n;
	   DblRasterMx kDiff;
	   DblRasterMx kFluv;
	   DblRasterMx kTect;
	   DblRasterMx mxLDD;
	   MultiflowDMatrix  mxMLDD;
	   MultiflowDMatrix  mxMLDD2;
	   MultiflowDMatrix mxSMLDD;
	   RasterPositionMatrix upstreamMaxAreaPosition;
	   RasterPositionMatrix downstreamMaxAreaPosition;
	   DblRasterMx mxDiagonal;
	   DblRasterMx upstreamMxDiagonal;
	   DblRasterMx streams;
	   DblRasterMx streams2;
	   DblRasterMx streams3;
	   DblRasterMx streams4;
	   DblRasterMx mxUpstreamMax;
	   DblRasterMx dAdL;
	   DblRasterMx dAdL2;
	   DblRasterMx upstreamDAdL;
	   DblRasterMx upstreamsumdAdL;
	   DblRasterMx mxUpstreamTotal;
	   DblRasterMx mxUpstreamRatio;
	   DblRasterMx perturbationsurface;	   
	   DblRasterMx logMxLongest;
	   DblRasterMx mxLongest;
	   DblRasterMx logMxAccflux;
	   DblRasterMx elongation;
	   DblRasterMx suspSedIn;
	   DblRasterMx suspSedOut;
	   DblRasterMx suspSedOutLocal;
	   DblRasterMx change2;
	   DblRasterMx mxDownstreamMax;
	   DblRasterMx mxDownstreamRatio;
	   DblRasterMx points;
	   DblRasterMx points2;
	   DblRasterMx path1;
	   DblRasterMx path2;
	   DblRasterMx catchment1;
	   DblRasterMx catchment2;
	   DblRasterMx catchment3;
	   DblRasterMx focusPoint;
	   DblRasterMx focusPoint2;
	   DblRasterMx focusPoint3;
	   //DblRasterMx yPositionFP;
	   //DblRasterMx zPositionFP;
	   DblRasterMx xCoordinate;
	   DblRasterMx yCoordinate;
	   DblRasterMx xCoordinate2;
	   DblRasterMx yCoordinate2;
	   DblRasterMx circle;
	   DblRasterMx circle2;
	   DblRasterMx distance;
	   DblRasterMx distance2;
	   DblRasterMx line1;
	   DblRasterMx line2;
	   DblRasterMx line3;
	   DblRasterMx line4;
	   DblRasterMx hollow;
	   DblRasterMx fpAccflux;
	   DblRasterMx fpSlope;
	   DblRasterMx fpLongest;
	   DblRasterMx fpYCoord;
	   DblRasterMx fpelo;
	   DblRasterMx fp2elo;
	   DblRasterMx fp2Accflux;
	   DblRasterMx fp2Slope;
	   DblRasterMx fp2Longest;
	   DblRasterMx fp2YCoord;
	   DblRasterMx fp3Accflux;
	   DblRasterMx fp3Slope;
	   DblRasterMx fp3Longest;
	   DblRasterMx upstreamSlope;
	   DblRasterMx dSdL;
	   DblRasterMx valve;
	   DblRasterMx streampower;
	   DblRasterMx downstreamDAdL;
	   DblRasterMx downstreamsumdAdL;
	   DblRasterMx streampowervalve;
	   DblRasterMx dAdLA;
	   DblRasterMx dSdLS;
	   DblRasterMx fpdAdLA;
	   DblRasterMx fpdSdLS;
	   DblRasterMx fpShortest;
	   DblRasterMx fp2dAdLA;
	   DblRasterMx fp2dSdLS;
	   DblRasterMx fp2Shortest;
	   DblRasterMx nDiff;
	   DblRasterMx crest;
	   DblRasterMx mxShortest;
	   DblRasterMx sumdAdL;
	   DblRasterMx sumdSdL;
	   DblRasterMx incision;
	   DblRasterMx fpsumdAdL;
	   DblRasterMx fp2sumdAdL;

	      

	   mapattr(nSizeY,nSizeX,dx,500, mx0);
	   mapattr(nSizeY,nSizeX,dx,0, mx3);
	   mapattr(nSizeY,nSizeX,dx,0, mxSlopeCorrected);
	   mapattr(nSizeY,nSizeX,dx,0, mxSlope);
	   mapattr(nSizeY,nSizeX,dx,100, tst2);
	   mapattr(nSizeY,nSizeX,dx,rand, tst3);
	   mapattr(nSizeY,nSizeX,dx,1, one);
	   mapattr(nSizeY,nSizeX,dx,-1, minusone);
	   mapattr(nSizeY,nSizeX,dx,0, zero);
	   mapattr(nSizeY,nSizeX,dx,50, fifty);
	   mapattr(nSizeY,nSizeX,dx,10, ten);
	   mapattr(nSizeY,nSizeX,dx,5, five);
	   mapattr(nSizeY,nSizeX,dx,0.5, half);
	   mapattr(nSizeY,nSizeX,dx,0.00001, zeroPlusABit);
	   mapattr(nSizeY,nSizeX,dx,100, mxEdges);
	   mapattr(nSizeY,nSizeX,dx,0.0, mxRandom);
	   mapattr(nSizeY,nSizeX,dx,0.0, mxRandom2);
	   //mapattr(nSizeY,nSizeX,dx,0.0, mxRandom2Max);
	   mapattr(nSizeY,nSizeX,dx,0.0, mxDownstreamMax);
	   mapattr(nSizeY,nSizeX,dx,0.0, mxUpstreamMin);
	   mapattr(nSizeY,nSizeX,dx,0.0, mxEroded);
	   mapattr(nSizeY,nSizeX,dx,0.0, sedOut);
	   mapattr(nSizeY,nSizeX,dx,0.0, sedOutWithoutDiff);
	   mapattr(nSizeY,nSizeX,dx,0.0, sedIn);
	   mapattr(nSizeY,nSizeX,dx,0.0, change);
	   //mapattr(nSizeY,nSizeX,dx,0.0, DiffSedOut);
	   //mapattr(nSizeY,nSizeX,dx,0.0, DiffSedIn);
	   mapattr(nSizeY,nSizeX,dx,0.0, lowerLimit);
	   mapattr(nSizeY,nSizeX,dx,0.0, upperLimit);
	   mapattr(nSizeY,nSizeX,dx,0, susp);					// set below!
	   mapattr(nSizeY,nSizeX,dx,kTectDouble, boundDecrease);
	   mapattr(nSizeY,nSizeX,dx,1, limitUpstream);
	   mapattr(nSizeY,nSizeX,dx,1.2, m);
	   mapattr(nSizeY,nSizeX,dx,1.0, n);
	   mapattr(nSizeY,nSizeX,dx,kDiffDouble, kDiff);    //!
	   mapattr(nSizeY,nSizeX,dx,kFluvDouble, kFluv);
	   mapattr(nSizeY,nSizeX,dx,kTectDouble, kTect);
       mapattr(nSizeY,nSizeX,dx,dx*dx, mxFluid);
	   mapattr(nSizeY,nSizeX,dx,0, mxDiagonal);
	   mapattr(nSizeY,nSizeX,dx,0, upstreamMxDiagonal);
	   mapattr(nSizeY,nSizeX,dx,0, streams);
	   mapattr(nSizeY,nSizeX,dx,0, streams2);
	   mapattr(nSizeY,nSizeX,dx,0, streams3);
	   mapattr(nSizeY,nSizeX,dx,0, streams4);
	   mapattr(nSizeY,nSizeX,dx,0, mxUpstreamMax);
	   mapattr(nSizeY,nSizeX,dx,0, dAdL);
	   mapattr(nSizeY,nSizeX,dx,0, dAdL2);
	   mapattr(nSizeY,nSizeX,dx,0, upstreamDAdL);
	    mapattr(nSizeY,nSizeX,dx,0, upstreamsumdAdL);
	   mapattr(nSizeY,nSizeX,dx,0, mxUpstreamTotal);	
	   mapattr(nSizeY,nSizeX,dx,0, mxUpstreamRatio);
	   mapattr(nSizeX,nSizeY,dx,0, perturbationsurface);	
	   mapattr(nSizeX,nSizeY,dx,0, logMxLongest);
	   mapattr(nSizeX,nSizeY,dx,0, mxLongest);
	   mapattr(nSizeX,nSizeY,dx,0, logMxAccflux);
	   mapattr(nSizeX,nSizeY,dx,0, elongation);
	   mapattr(nSizeX,nSizeY,dx,0, suspSedIn);
	   mapattr(nSizeX,nSizeY,dx,0, suspSedOut);
	   mapattr(nSizeX,nSizeY,dx,0, suspSedOutLocal);
	   mapattr(nSizeX,nSizeY,dx,0, change2);
	   mapattr(nSizeY,nSizeX,dx,0, mxDownstreamMax);
	   mapattr(nSizeY,nSizeX,dx,0, mxDownstreamRatio);
	   mapattr(nSizeY,nSizeX,dx,0, points);
	   mapattr(nSizeY,nSizeX,dx,0, points2);
	   mapattr(nSizeY,nSizeX,dx,0, path1);
	   mapattr(nSizeY,nSizeX,dx,0, path2);
	   mapattr(nSizeY,nSizeX,dx,0, catchment1);
	   mapattr(nSizeY,nSizeX,dx,0, catchment2);
	   mapattr(nSizeY,nSizeX,dx,0, catchment3);
	   mapattr(nSizeY,nSizeX,dx,0, mxAccflux);
	   mapattr(nSizeY,nSizeX,dx,0, mxAccflux2);
	   mapattr(nSizeY,nSizeX,dx,0, mxAccflux3);
	   mapattr(nSizeY,nSizeX,dx,0, focusPoint);
	   mapattr(nSizeY,nSizeX,dx,0, focusPoint2);
	   mapattr(nSizeY,nSizeX,dx,0, focusPoint3);
	   //mapattr(nSizeY,nSizeX,dx,0, yPositionFP);
	   //mapattr(nSizeY,nSizeX,dx,0, zPositionFP);
	   mapattr(nSizeY,nSizeX,dx,0, xCoordinate);
	   mapattr(nSizeY,nSizeX,dx,0, yCoordinate);
	   mapattr(nSizeY,nSizeX,dx,0, xCoordinate2);
	   mapattr(nSizeY,nSizeX,dx,0, yCoordinate2);
	   mapattr(nSizeY,nSizeX,dx,0, circle);
	   mapattr(nSizeY,nSizeX,dx,0, circle2);
	   mapattr(nSizeY,nSizeX,dx,0, distance);
	   mapattr(nSizeY,nSizeX,dx,0, distance2);
	   mapattr(nSizeY,nSizeX,dx,0, line1);
	   mapattr(nSizeY,nSizeX,dx,0, line2);
	   mapattr(nSizeY,nSizeX,dx,0, line3);
	   mapattr(nSizeY,nSizeX,dx,0, line4);
	   mapattr(nSizeY,nSizeX,dx,0, hollow);
	   mapattr(nSizeY,nSizeX,dx,0, fpAccflux);
	   mapattr(nSizeY,nSizeX,dx,0, fpSlope);
	   mapattr(nSizeY,nSizeX,dx,0, fpLongest);
	   mapattr(nSizeY,nSizeX,dx,0, fpYCoord);
	   mapattr(nSizeY,nSizeX,dx,0, fpelo);
	   mapattr(nSizeY,nSizeX,dx,0, fp2elo);
	   mapattr(nSizeY,nSizeX,dx,0, fpShortest);
	   mapattr(nSizeY,nSizeX,dx,0, fp2Shortest);
	   mapattr(nSizeY,nSizeX,dx,0, fp2Accflux);
	   mapattr(nSizeY,nSizeX,dx,0, fp2Slope);
	   mapattr(nSizeY,nSizeX,dx,0, fp2Longest);
	   mapattr(nSizeY,nSizeX,dx,0, fp2YCoord);
	   mapattr(nSizeY,nSizeX,dx,0, fp3Accflux);
	   mapattr(nSizeY,nSizeX,dx,0, fp3Slope);
	   mapattr(nSizeY,nSizeX,dx,0, fp3Longest);
	   mapattr(nSizeY,nSizeX,dx,0, upstreamSlope);
	   mapattr(nSizeY,nSizeX,dx,0, dSdL);
	   mapattr(nSizeY,nSizeX,dx,0, valve);
	   mapattr(nSizeY,nSizeX,dx,0, streampower);
	   mapattr(nSizeY,nSizeX,dx,0, downstreamDAdL);
	   mapattr(nSizeY,nSizeX,dx,0, downstreamsumdAdL);
	   mapattr(nSizeY,nSizeX,dx,0, streampowervalve);
	   mapattr(nSizeY,nSizeX,dx,0, dAdLA);
	   mapattr(nSizeY,nSizeX,dx,0, dSdLS);
	   mapattr(nSizeY,nSizeX,dx,0, fpdAdLA);
	   mapattr(nSizeY,nSizeX,dx,0, fpdSdLS);
	   mapattr(nSizeY,nSizeX,dx,0, fp2dAdLA);
	   mapattr(nSizeY,nSizeX,dx,0, fp2dSdLS);
	   mapattr(nSizeY,nSizeX,dx,0, nDiff);
	   mapattr(nSizeY,nSizeX,dx,0, crest);
	   mapattr(nSizeY,nSizeX,dx,0, mxShortest);
	   mapattr(nSizeY,nSizeX,dx,0, sumdAdL);
	   mapattr(nSizeY,nSizeX,dx,0, sumdSdL);
	   mapattr(nSizeY,nSizeX,dx,0, incision);
	   mapattr(nSizeY,nSizeX,dx,0, fpsumdAdL);
	   mapattr(nSizeY,nSizeX,dx,0, fp2sumdAdL);
	      


	   for ( j = 0; j < nSizeX; j++ ){
		   for ( l = 0; l < nSizeY; l++){
			 if ( l==0 ){
				  mxFluid(l,j) = dx*dx*1;
			 } else {
				//   if ( l > j - 20 | l > -j + 19 | j==20 | j==19 ){
				//	   mxFluid(l,j) = 1;
				//   } else {
					   mxFluid(l,j) = dx*dx;
				//   }
			  }
		   }
	   }
	   //tangent = U * 1;

	   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){
							if (l > slopeBreak - 1){
								mx0(l,j) = 0 + (nSizeY-l) * tangent * dx;
							} else {
								mx0(l,j) = (nSizeY - slopeBreak) * tangent * dx + (slopeBreak-l) * tangent * 1 * dx;
							}
						}
			   }
	   uniform(mxRandom);
	   /*for ( j = 0; j < nSizeX; j++ ){
		   for ( l = 0; l < nSizeY; l++ ){
			   if (l < slopeBreak) {
				   tst3(l,j) = 1 * tst3(l,j);
			   }
			   if (l > nSizeY-2){
				   tst3(l,j) = 0;
			   }
			}
	   }*/
	   mx1 = mx0 + mxRandom * tst3 + tst2; //- tst3 * 0.5;
   
	   saveToAsc(mx1,0,"startTerrain");
	 
 
	   loadFromAsc("d:\\pcrinst\\mx1200000.asc",mx1,10);
	   //loadFromAsc("d:\\terRAIN\\test\\mx1020000.asc",mx1,10);
	   //loadFromAsc("d:\\pcrinst\\borzsony50.asc",mx1,10);
	   //printmx(mx3); 
	   //mx1 = mx1 + tst2;
	   mxEdges = mx1;


					statFile.print("yPositionFP");
					statFile.print("zPositionFP");
					statFile.print("depthFP");
					statFile.print("focusPointMigration");
					statFile.print("pathsum");
					statFile.print("path1sum");
					statFile.print("path2sum");
					statFile.print("areaCatchment3");
					statFile.print("hollowShape");
					statFile.print("catchmentShape");
					statFile.print("widthRatio");
					statFile.print("hollowWidth");
					statFile.print("catchment1Width");
					statFile.endl();
	   
	  
	   for (i = 1; i <= iterationNumber; i++) {

			   std::cout<<"Iteration nr: "<<i<<std::endl;
			   
			   
			   /*if (( i % 1000 )==0){	
					focusPointMigration = sqrt((yPositionFPOld - yPositionFP)*(yPositionFPOld - yPositionFP) + (zPositionFPOld - zPositionFP)*(zPositionFPOld - zPositionFP));
					yPositionFPOld = yPositionFP;
					zPositionFPOld = zPositionFP;
				}*/
			   
					//statFile.print("szöveg");
					// az elsõ pixel:
					//statFile.print(mx1(49,0));
					// a mátrix átlaga:
					
				if (( i % 100 )==0){	
					//double average = averageNotZeroItems(mx1); 
					//statFile.print(average);
					statFile.print(yPositionFP);
					statFile.print(zPositionFP);
					statFile.print(depthFP);
					statFile.print(focusPointMigration);
					statFile.print(pathsum);
					statFile.print(path1sum);
					statFile.print(path2sum);
					statFile.print(areaCatchment3);
					statFile.print(hollowShape);
					statFile.print(catchmentShape);
					statFile.print(widthRatio);
					statFile.print(hollowWidth);
					statFile.print(catchment1Width);
					//statFile.print(yCoordCircleLimit);
					//statFile.print(xCoordCircleMax);
					//statFile.print(c1p1x);
					//statFile.print(c1p2x);
					//statFile.print(c1p3x);
					//statFile.print(c1p4x);
					// új sort kezd:
					statFile.endl();
				}

			   pathsum=0;
			      path1sum=0;
				     path2sum=0;
			   areaCatchment3=0;
			   catchment1Width = 0;
			   hollowWidth = 0;
			   widthRatio = 0;
			   hollowShape = 0;
			   catchmentShape = 0;
			   depthFP = 0;
			   xCoordCircleMax = 0;
			   yCoordCircleLimit = 0;
			   yCoordCircleMin = (nSizeY-1)*dx;
			    c1p1x = nSizeX*dx;				
				c1p2x = 0;				
				c1p3x = 0;				
				c1p4x = 0;	
				c1p5x = 0;
				c1p6x = 0;
				h1p1x = nSizeX * dx;
				h1p2x = 0;

			  		   
		
			   
               slope(mx1,mxSlope); 
			   max(mxSlope,zeroPlusABit, mxSlopeCorrected);
			   lddcreate(mx1, mxLDD, true);
			   multiflowLDD( one, mx1, mxMLDD,true);
			   semiMultiflowLDD(mx1, mxSMLDD, true);
			   diagonal(mxMLDD, mxDiagonal);
			   accflux(mxMLDD,mxFluid,mxAccflux,accuFluxLimitRate);					  
			   longestflowpathlength(mxSMLDD, mxLongest);
			   max(mxLongest, five, mxLongest);
			   log10(mxLongest, logMxLongest);
			   log10(mxAccflux, logMxAccflux);
			   elongation = logMxLongest / logMxAccflux;	   
			   upstreammax( mxMLDD, mxAccflux, mxUpstreamMax, upstreamMaxAreaPosition, true);
			   downstreammax( mxMLDD, mxAccflux, mxDownstreamMax, downstreamMaxAreaPosition, true);
			   upstreamtotal(mxMLDD, mxAccflux, mxUpstreamTotal, true,true);	
			   diagonal(upstreamMaxAreaPosition, upstreamMxDiagonal);
			   dAdL = (mxAccflux - mxUpstreamMax) * upstreamMxDiagonal;
			   //max(dAdL, fifty, dAdL);
			   getMatrixPosItems(mxSlopeCorrected,upstreamMaxAreaPosition,upstreamSlope);
			   dSdL = (mxSlopeCorrected - upstreamSlope) * upstreamMxDiagonal;
			   sum_of_upstreamdiffs(mxMLDD, mxAccflux, sumdAdL, 0.0);
			   sum_of_upstreamdiffs(mxMLDD, mxSlopeCorrected, sumdSdL, 0.0);
			   getMatrixPosItems(sumdAdL,upstreamMaxAreaPosition,upstreamsumdAdL);  
			   //dAdL2 = (dAdL - upstreamDAdL) * upstreamMxDiagonal; 	
			   dAdLA = m * sumdAdL / mxAccflux;
			   dSdLS = n * sumdSdL / mxSlopeCorrected;
			   nDiff = dSdLS / dAdLA;
			   valve = dAdLA + dSdLS;
			   streampower = mxAccflux^m * mxSlopeCorrected^n;
			   streampowervalve = streampower * valve;
			   getMatrixPosItems(sumdAdL,downstreamMaxAreaPosition,downstreamsumdAdL);
			    for ( j = 0; j < nSizeX; j++ ){
						for ( l = 1; l < nSizeY; l++ ){
							crest(l,j) = 0;
							if (mxAccflux (l,j) < 200){   
								crest(l,j) = 1;
							}
						}
			   }
			   spreadLDD(mxMLDD,crest,mxShortest,0.0);
			   spreadLDDMax( mxSMLDD, crest, mxLongest,0.0 );
			   incision = mxLongest - mxShortest;

			   //points2(nSizeY-1,10) = 1;
			   //points2 = points2; //+ path1;
			   //catchment(mxSMLDD,points2,catchment1);

			   
			      

			   // identify streams via suspension ratios
			   //for ( j = 0; j < nSizeX; j++ ){
						//for ( l = 1; l < nSizeY; l++ ){
						//	if (mxUpstreamTotal (l,j) == 0){   // | mxUpstreamMax(l,j) == mxUpstreamTotal(l,j) ){   // | mxAccflux(l,j) < 5
						//		mxUpstreamRatio(l,j) = 0;
						//	} else {
						//		mxUpstreamRatio(l,j) = mxUpstreamMax(l,j) / mxUpstreamTotal(l,j);
						//	}
						//}
			   //}
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){
							mxDownstreamRatio(l,j) = mxDownstreamMax(l,j) / mxAccflux(l,j);
							}
			   }
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){
							streams(l,j) = 0;
							streams2(l,j) = 0;
							}
			   }
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){
							susp(l,j) = 1;
							}
			   }
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){									
							if ( mxDownstreamRatio(l,j) > 0.6 && downstreamsumdAdL(l,j) < sumdAdL(l,j)){		// mxUpstreamRatio(l,j) > 0.1 &&						
								streams(l,j) = 1; //0.5;                 						
							}
						}
			   }			  
			   
			   path (mxSMLDD, streams, path1);
			   for ( j = 0; j < nSizeX; j++ ){			// because of the unincluded first pixel
						for ( l = 0; l < nSizeY; l++ ){			
							if (path1(l,j) == 1 | streams(l,j) == 1){
								path1(l,j) = 1;                 
							}
						}
			   }
			   for ( j = 0; j < nSizeX; j++ ){			
						for ( l = 0; l < nSizeY; l++ ){			
							path1sum = path1(l,j) + path1sum;							
						}
			   }

			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){									
							if (mxDownstreamRatio(l,j) > 0.6){			//mxUpstreamRatio(l,j) > 0.1 && 					
								streams2(l,j) = 1; //0.5;                 						
							}
						}
			   }			  
			   path (mxSMLDD, streams2, path2);
			   for ( j = 0; j < nSizeX; j++ ){			// because of the unincluded first pixel
						for ( l = 0; l < nSizeY; l++ ){			
							if (path2(l,j) == 1 | streams2(l,j) == 1){
								path2(l,j) = 1;                 
							}
						}
			   }
			      for ( j = 0; j < nSizeX; j++ ){			
						for ( l = 0; l < nSizeY; l++ ){			
							path2sum = path2(l,j) + path2sum;							
						}
			   }


			   for ( j = 0; j < nSizeX; j++ ){		// set suspension
						for ( l = 0; l < nSizeY; l++ ){			
							if (path1(l,j) == 1){
								susp(l,j) = 1.0;                 
							} else {
								susp(l,j) = 1;
							}
						}
			   }
			   /*for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){			
							yCoordinate(l,j) = ((l+1) * dx) - (dx/2);                 
						}
			   }
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){			
							xCoordinate(l,j) = ((j+1) * dx) - (dx/2);                 
						}
			   }*/
			   

			   accflux(mxLDD,path1,mxAccflux2);
			   accflux(mxLDD,path2,mxAccflux3);
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){			
							focusPoint(l,j) = 0; 
							focusPoint2(l,j) = 0; 
							fpAccflux(l,j)=0;
							fpSlope(l,j)=0;
							fpLongest(l,j)=0;	
							fpYCoord(l,j)=0;
							fpdAdLA(l,j)=0;
							fpdSdLS(l,j)=0;
							fpelo(l,j)=0;
							fpShortest(l,j)=0;
							fp2Accflux(l,j)=0;
							fp2Slope(l,j)=0;
							fp2Longest(l,j)=0;	
							fp2Shortest(l,j)=0;	
							fp2YCoord(l,j)=0;
							fp2elo(l,j)=0;
							fp2dAdLA(l,j)=0;
							fp2dSdLS(l,j)=0;
							fp3Accflux(l,j)=0;
							fp3Slope(l,j)=0;
							fp3Longest(l,j)=0;	
							fpsumdAdL(l,j)=0;	
							fp2sumdAdL(l,j)=0;	
						}
			   }
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){			
							//if (mxAccflux2(l,j) == 1){
							if (DoubleUtil::cmpD6(mxAccflux2(l,j),1.0)){
								focusPoint(l,j) = 1; 
								//yPositionFP = yCoordinate(l,j) / yCoordinate(nSizeY-1,j);  //nSizeY;
								//zPositionFP = mx1(l,j)/(mx1(0,1)-mx1(nSizeY,1));
								//xCoordFP = (j+1)*dx - dx/2;
								//yCoordFP = (l+1)*dx - dx/2;
								fpAccflux(l,j)=mxAccflux(l,j);
								fpSlope(l,j)=mxSlope(l,j);
								fpLongest(l,j)=mxLongest(l,j);	
							    fpdAdLA(l,j)=dAdLA(l,j);
								fpdSdLS(l,j)=dSdLS(l,j);
								fpelo(l,j)=elongation(l,j);
								fpShortest(l,j)=mxShortest(l,j);
								fpsumdAdL(l,j)=sumdAdL(l,j);
								//fpYCoord(l,j)=yCoordinate(l,j);	*/
							} 
							//if (DoubleUtil::cmpD6(mxAccflux2(l,j),2.0)){
							if (DoubleUtil::cmpD6(mxAccflux3(l,j),1.0)){
								focusPoint2(l,j) = 1;
								fp2Accflux(l,j)=mxAccflux(l,j);
								fp2Slope(l,j)=mxSlope(l,j);
								fp2Longest(l,j)=mxLongest(l,j);	
								fp2dAdLA(l,j)=dAdLA(l,j);
								fp2dSdLS(l,j)=dSdLS(l,j);
								fp2elo(l,j)=elongation(l,j);
								fp2Shortest(l,j)=mxShortest(l,j);
								fp2sumdAdL(l,j)=sumdAdL(l,j);
								//fp2YCoord(l,j)=yCoordinate(l,j);	*/
							} 
							//if (DoubleUtil::cmpD6(mxAccflux2(l,j),2.0)){
							//	focusPoint3(l,j) = 1;
							//	fp3Accflux(l,j)=mxAccflux(l,j);
							//	fp3Slope(l,j)=mxSlope(l,j);
							//	fp3Longest(l,j)=mxLongest(l,j);	
							//	//fp3Coord(l,j)=yCoordinate(l,j);	*/
							//} 
						}
			   }		
			    						
			    //sedOut=kFluv*tst2*tst3;			   	
			   sedOut = kDiff*mxSlopeCorrected + kFluv*(mxSlopeCorrected^n)*(mxAccflux^m);
			   /*sedOutWithoutDiff = kFluv*(mxSlopeCorrected^n)*(mxAccflux^m);
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){
							if (streams(l,j) == 1){
								sedOut(l,j) = sedOutWithoutDiff(l,j); 	
							} else {
								sedOut(l,j) = sedOut(l,j); 
							}
						}
			   }*/
    		 
			  
			   upstreamtotal(mxMLDD, sedOut, sedIn, true,true);				 
			   change = sedIn * susp - sedOut * mxDiagonal;		   
			   //change = minusone * kFluv * streampowervalve - kDiff * mxSlopeCorrected;
			   //erosion=sedOut-sedIn*susp;
			   //downstreammax( mxMLDD, erosion, erosionDownstreamMax);
			   //erosionDifference=erosion-erosionDownstreamMax;
			   //heightDifference=mx1-mxDownstreamMax;
			   //mx1 = mx1 - sedOut + sedIn*susp;
			   //upperLimit = mx1+limitUpstream;
			   //lowerLimit = mx1-limitUpstream;
			   //mx1 = mx1 + change;
			   //min( mx1, upperLimit, mx1);
			   //max( mx1, lowerLimit, mx1); 


				// clean edges
			  /* for ( j = 0; j < nSizeY; j++ ){
                       valve(j,nSizeX-1) = zero(j,nSizeX-1);
                       valve(j,0) = zero(j,0);
               }
               for ( j = 1; j < nSizeX-1; j++ ){
                       valve(nSizeY-1,j) = zero(nSizeY-1,j);
                       valve(0,j) = zero(0,j);
               }
			   for ( j = 0; j < nSizeY; j++ ){
                       change(j,nSizeX-1) = -kTect(j,nSizeX-1);
                       change(j,0) = -kTect(j,0);
               }
               for ( j = 1; j < nSizeX-1; j++ ){
                       change(nSizeY-1,j) = -kTect(nSizeY-1,j);
                       change(0,j) = -kTect(0,j);
               }
			   for ( j = 0; j < nSizeY; j++ ){
                       dSdL(j,nSizeX-1) = zero(j,nSizeX-1);
                       dSdL(j,0) = zero(j,0);
               }
               for ( j = 1; j < nSizeX-1; j++ ){
                       dSdL(nSizeY-1,j) = zero(nSizeY-1,j);
                       dSdL(0,j) = zero(0,j);
               }
			   for ( j = 0; j < nSizeY; j++ ){
                       streampowervalve(j,nSizeX-1) = zero(j,nSizeX-1);
                       streampowervalve(j,0) = zero(j,0);
               }
               for ( j = 1; j < nSizeX-1; j++ ){
                       streampowervalve(nSizeY-1,j) = zero(nSizeY-1,j);
                       streampowervalve(0,j) = zero(0,j);
               }
			   for ( j = 0; j < nSizeY; j++ ){
                       dSdLS(j,nSizeX-1) = zero(j,nSizeX-1);
                       dSdLS(j,0) = zero(j,0);
               }
               for ( j = 1; j < nSizeX-1; j++ ){
                       dSdLS(nSizeY-1,j) = zero(nSizeY-1,j);
                       dSdLS(0,j) = zero(0,j);
               }
			    for ( j = 0; j < nSizeY; j++ ){
                       nDiff(j,nSizeX-1) = zero(j,nSizeX-1);
                       nDiff(j,0) = zero(j,0);
               }
               for ( j = 1; j < nSizeX-1; j++ ){
                       nDiff(nSizeY-1,j) = zero(nSizeY-1,j);
                       nDiff(0,j) = zero(0,j);
               }*/

			   //for ( j = 0; j < nSizeX; j++ ){
						//for ( l = 0; l < nSizeY-1; l++ ){			// one side only!!
						//	//if (catchment1(l,j) == 1){ 
						//	if (mxLDD(l,j) == 5) {
						//	   mx1(l,j) = mx1(l,j) + sedIn(l,j) * susp(l,j); // - kTect(l,j);   // reflecting boundary
						//   } else {
						//		mx1(l,j) = mx1(l,j) + change(l,j); // + kTect(l,j);
						//   }							
						//}
			   //}

			   //if (i < 200000) {
				   mxEdges = mxEdges-kTect;
			   //}
			   ////// lower Edges - infinite sink
			   for ( j = 0; j < nSizeY; j++ ){
                       mx1(j,nSizeX-1) = mxEdges(j,nSizeX-1);
					   //mx1(j,nSizeX-2) = mxEdges(j,nSizeX-2);
                       mx1(j,0) = mxEdges(j,0);
					   //mx1(j,1) = mxEdges(j,1);
               }

               for ( j = 1; j < nSizeX-1; j++ ){
                       mx1(nSizeY-1,j) = mxEdges(nSizeY-1,j);
					   //mx1(nSizeY-2,j) = mxEdges(nSizeY-2,j);
                       mx1(0,j) = mxEdges(0,j);
					   //mx1(1,j) = mxEdges(1,j);
               }
			   
			  ////  //lower bottomSide
			  //for ( j = 0; j < nSizeX; j++ ){
     //                  mx1(nSizeY-1,j)= mxEdges(nSizeY-1,j);
					////   //mx1(nSizeY-1,j)= mx1(nSizeY-1,j)-boundDecrease(nSizeY-1,j);
     //         }
			   
			   //  //lower bottomSide and topSide
			  //for ( j = 0; j < nSizeX; j++ ){
     //                  mx1(nSizeY-1,j)= mxEdges(nSizeY-1,j);
					//   mx1(0,j)= mxEdges(0,j);
					//   //mx1(nSizeY-1,j)= mx1(nSizeY-1,j)-boundDecrease(nSizeY-1,j);
     //          }

			   //// lower bottomPlain
			   //for ( j = 0; j < nSizeX; j++ ){
				  // for ( l = nSizeY-1; l > nSizeY-1-20; l-- ){
					 //  //if (l > nSizeY-1-1){
						//   //mx1(l,j)= mxEdges(l,j); //mx1(l,j)- boundDecrease(l,j);
					 //  //} else {
						//   mx1(l,j)= mx1(l,j)-kTect(l,j);
						//   //mx1(nSizeY-1,j) = mx1(l,j);
					 //  //}					   		   
				  // }
      //         }


			   // lower bottomLeft
			   //mx1(nSizeY-1,0) = mxEdges(nSizeY-1,0);

			 if (( i % 1000 )==0){
					saveToAsc(mx1,i,"mx2",true);
					saveToAsc(mx1,i,"mx1");
					saveToAsc(mxAccflux,i,"accflux",true);
					saveToAsc(mxSlopeCorrected,i,"slope",true);
					saveToAsc(sedOut, i, "sedOut", true);
					saveToAsc(sedIn, i, "sedIn", true);
					saveToAsc(change, i, "change",true);
					saveToAsc(mxLDD, i, "mxLDD");
					saveToAsc(mxMLDD, i, "mxMLDD");
					saveToAsc(mxDiagonal, i, "mxDiagonal");
					saveToAsc(streams, i, "streams");
					saveToAsc(susp, i, "susp");
					saveToAsc(mxUpstreamMax, i, "mxUpstreamMax");
					saveToAsc(crest, i, "crest",true);
					saveToAsc(mxUpstreamRatio, i, "mxUpstreamRatio");
					saveToAsc(upstreamMaxAreaPosition,i,"upstreamMaxAreaPosition");
					saveToAsc(mxDiagonal, i, "mxDiagonal");
					saveToAsc(suspSedOut, i, "suspSedOut");
					saveToAsc(suspSedIn, i, "suspSedIn");
					saveToAsc(suspSedOutLocal, i, "suspSedOutLocal");
					saveToAsc(change2, i, "change2");
					saveToAsc(mxFluid, i, "mxFluid");
					saveToAsc(mxDownstreamMax, i, "mxDownstreamMax");
					saveToAsc(mxDownstreamRatio, i, "mxDownstreamRatio",true);
					saveToAsc(points, i, "points");
					saveToAsc(points2, i, "points2");
					saveToAsc(path1, i, "path1",true);
					saveToAsc(path2, i, "path2",true);
					saveToAsc(catchment1, i, "catchment1");
					saveToAsc(catchment3, i, "catchment3");
					saveToAsc(streams2, i, "streams2",true);
					saveToAsc(downstreamDAdL, i, "downstreamDAdL");
					saveToAsc(dAdL2, i, "dAdL2");
					saveToAsc(focusPoint, i, "focusPoint");
					saveToAsc(focusPoint2, i, "focusPoint2");
					saveToAsc(focusPoint3, i, "focusPoint3");
					saveToAsc(catchment2, i, "catchment2");
					saveToAsc(yCoordinate, i, "yCoordinate");
					saveToAsc(xCoordinate, i, "xCoordinate");
					saveToAsc(circle2, i, "circle2");
					saveToAsc(distance, i, "distance");
					saveToAsc(distance2, i, "distance2");
					saveToAsc(line1, i, "line1");
					saveToAsc(hollow, i, "hollow");
					saveToAsc(fpAccflux, i, "fpAccflux",true);
					saveToAsc(fpSlope, i, "fpSlope",true);
					saveToAsc(fpLongest, i, "fpLongest",true);
					saveToAsc(fpYCoord, i, "fpYCoord");
					saveToAsc(fpelo, i, "fpelo",true);
					saveToAsc(fpShortest, i, "fpShortest",true);
					saveToAsc(fp2elo, i, "fp2elo",true);
					saveToAsc(fp2Shortest, i, "fp2Shortest",true);
					saveToAsc(fp2Accflux, i, "fp2Accflux",true);
					saveToAsc(fp2Slope, i, "fp2Slope",true);
					saveToAsc(fp2Longest, i, "fp2Longest",true);
					saveToAsc(fp2YCoord, i, "fp2YCoord");
					saveToAsc(fp3Accflux, i, "fp3Accflux");
					saveToAsc(fp3Slope, i, "fp3Slope");
					saveToAsc(fp3Longest, i, "fp3Longest");
					saveToAsc(dAdL, i, "dAdL",true);
					saveToAsc(upstreamDAdL, i, "upstreamDAdL");
					saveToAsc(dAdL2, i, "dAdL2");
					saveToAsc(upstreamSlope, i,"upstreamSlope");
					saveToAsc(dSdL, i, "dSdL",true);
					saveToAsc(valve, i, "valve",true);
				    saveToAsc(streampower, i, "streampower",true);
					saveToAsc(streampowervalve, i, "streampowervalve",true);
					saveToAsc(dAdLA, i, "dAdLA",true);
					saveToAsc(dSdLS, i, "dSdLS",true);
				    saveToAsc(fpdAdLA, i, "fpdAdLA",true);
					saveToAsc(fpdSdLS, i, "fpdSdLS",true);
					saveToAsc(fp2dAdLA, i, "fp2dAdLA",true);
					saveToAsc(fp2dSdLS, i, "fp2dSdLS",true);
					saveToAsc(nDiff, i, "nDiff",true);
					saveToAsc(mxLongest, i, "mxLongest",true);
					saveToAsc(mxShortest, i, "mxShortest",true);
					saveToAsc(sumdAdL, i, "sumdAdL",true);
					saveToAsc(sumdSdL, i, "sumdSdL",true);
					saveToAsc(elongation, i, "elongation",true);
					saveToAsc(incision, i, "incision",true);
					saveToAsc(fpsumdAdL, i, "fpsumdAdL",true);
					saveToAsc(fp2sumdAdL, i, "fp2sumdAdL",true);
			   }               
       }

	   saveToAsc(mx1,0,"result");
	   printmx(mx1);


	
	return true;
}

};





//{
//	/*
//	LEMParams & params = static_cast<LEMParams &>(_params);
//	setOutputDirectory(params.getOutputDirectory().c_str());
//	setOutflowType(params.getOutflowType());
//    */
//
//   // output directory: save every output into this directory
//	   //setOutputDirectory("d:\\terRAIN\\test");
//	setOutputDirectory("d:\\pcrinst");
//	   /*
//	ofTopLeft ,
//	ofTopRight,
//	ofBottomLeft,
//	ofBottomRight,
//	ofTopSide,
//	ofRightSide,
//	ofBottomSide,
//	ofLeftSide,
//	ofAllSides,
//	ofTopAndBottomSide,
//	ofLeftAndRightSide
//	ofTopAndMiddleOfBottom
//	*/
//	setOutflowType(ofBottomSide);
//	//setOutflowType(ofTopAndBottomSide);
//	//setOutflowType(ofAllSides);
//	//setOutflowType(ofBottomLeft);
//	//setOutflowType(ofTopAndMiddleOfBottom);
//	
//	
//	   // model size:
//	   size_t nSizeX = 60;  //797;
//	   size_t nSizeY = 60;  //788;
//	   size_t slopeBreak = 50;		// can not be zero!
//	   // statfile:
//	   StatFile statFile("statfile",15);
//	   StatFile statFile2("statfile2",15);
//	   // in the accuflux function neglet the flow if account is less then 
//	   // initial flow account*accuFluxLimitRate
//	   double dx = 10;
//	   double tangent = 2.0;
//	   double kDiffDouble = 0e-3;
//	   double kFluvDouble = 1e-5;
//	   double kTectDouble = 1e-3;				// set below too!
//	   double accuFluxLimitRate = 0.0;
//	   double mxRandom2Max = 0;
//	   double mxAccfluxMax = 0;
//	   double mxLocalMin = 0;
//	   //double limitUpstream = 0.01;
//	   // decrease the boundary pixels in every iteration with this value:
//	   //double boundDecrease = 0.01;
//	   //double Kfluv = 0.1;
//	   //double rain = 0.01;
//	   double rand = 0.000001;
//	   //double Susp = 0;
//	   // 1 for TRL, 0 for DTL
//	   double xCoordFP = 0;
//	   double yCoordFP = 0;
//	   //double zCoordFP = 0;
//	   double yPositionFP = 0;
//	   double zPositionFP = 0;
//	   double yPositionFPOld = 0;
//	   double zPositionFPOld = 0;
//	   double focusPointMigration = 0;
//	   double xCoordCircleMax = 0;
//	   double yCoordCircleLimit = 0;
//	   double yCoordCircleMin = (nSizeY-1)*dx;
//	   double c1p1x = nSizeX*dx;	   
//	   double c1p2x = 0;	   
//	   double c1p3x = 0;	   
//	   double c1p4x = 0;
//	   double c1p5x = 0;	   
//	   double c1p6x = 0;
//	   double h1p1x = nSizeX*dx;	   
//	   double h1p2x = 0;	   
//	   double catchment1Width = 0;
//	   double hollowWidth = 0;
//	   double a = 1;
//	   double a2 = 0.3;
//	   double hollowShape = 0;
//	   double catchmentShape = 0;
//	   double depthFP = 0;
//	   double widthRatio = 0;
//
//
//	   // number of iterations:
//	   size_t iterationNumber = 1;
//	   size_t i = 1;
//       size_t j = 0;
//	   size_t l = 0;
//	   size_t lmax = 0;
//       size_t jmax = 0;
//	   size_t l2 = 0;
//       size_t j2 = 0;
//	   size_t pathsum = 0;
//	   size_t areaCatchment3 = 0;
//	   	   
//       DblRasterMx mx0;
//	   DblRasterMx mx1;
//	   DblRasterMx mx3;
//	   DblRasterMx tst2;
//	   DblRasterMx tst3;
//	   DblRasterMx one;
//	   DblRasterMx minusone;
//	   DblRasterMx zero;
//	   DblRasterMx fifty;
//	   DblRasterMx ten;
//	   DblRasterMx five;
//	   DblRasterMx half;
//	   DblRasterMx zeroPlusABit;
//	   DblRasterMx mxEdges;
//       DblRasterMx mxRandom;
//       DblRasterMx mxRandom2;
//	   //DblRasterMx mxRandom2Max;
//	   DblRasterMx mxSlope;
//	   DblRasterMx mxSlopeCorrected;
//       DblRasterMx mxAccflux;
//	   DblRasterMx mxAccflux2;
//	   DblRasterMx mxAccflux3;
//       DblRasterMx mxFluid;
//	   DblRasterMx change;
//	   DblRasterMx mxUpstreamMin;
//	   DblRasterMx lowerLimit;
//	   DblRasterMx upperLimit;
//	   DblRasterMx sedOut;
//	   DblRasterMx sedOutWithoutDiff;
//	   DblRasterMx sedIn;
//	   //DblRasterMx DiffSedOut;
//	   //DblRasterMx DiffSedIn;
//	   DblRasterMx mxEroded;
//	   DblRasterMx susp;
//	   DblRasterMx boundDecrease;
//	   DblRasterMx limitUpstream;
//	   DblRasterMx m;
//	   DblRasterMx n;
//	   DblRasterMx kDiff;
//	   DblRasterMx kFluv;
//	   DblRasterMx kTect;
//	   DblRasterMx mxLDD;
//	   MultiflowDMatrix  mxMLDD;
//	   MultiflowDMatrix  mxMLDD2;
//	   MultiflowDMatrix mxSMLDD;
//	   RasterPositionMatrix upstreamMaxAreaPosition;
//	   RasterPositionMatrix downstreamMaxAreaPosition;
//	   DblRasterMx mxDiagonal;
//	   DblRasterMx upstreamMxDiagonal;
//	   DblRasterMx streams;
//	   DblRasterMx streams2;
//	   DblRasterMx streams3;
//	   DblRasterMx streams4;
//	   DblRasterMx mxUpstreamMax;
//	   DblRasterMx dAdL;
//	   DblRasterMx dAdL2;
//	   DblRasterMx upstreamDAdL;
//	   DblRasterMx upstreamsumdAdL;
//	   DblRasterMx mxUpstreamTotal;
//	   DblRasterMx mxUpstreamRatio;
//	   DblRasterMx perturbationsurface;	   
//	   DblRasterMx logMxLongest;
//	   DblRasterMx mxLongest;
//	   DblRasterMx logMxAccflux;
//	   DblRasterMx elongation;
//	   DblRasterMx suspSedIn;
//	   DblRasterMx suspSedOut;
//	   DblRasterMx suspSedOutLocal;
//	   DblRasterMx change2;
//	   DblRasterMx mxDownstreamMax;
//	   DblRasterMx mxDownstreamRatio;
//	   DblRasterMx points;
//	   DblRasterMx points2;
//	   DblRasterMx path1;
//	   DblRasterMx path2;
//	   DblRasterMx catchment1;
//	   DblRasterMx catchment2;
//	   DblRasterMx catchment3;
//	   DblRasterMx focusPoint;
//	   DblRasterMx focusPoint2;
//	   DblRasterMx focusPoint3;
//	   //DblRasterMx yPositionFP;
//	   //DblRasterMx zPositionFP;
//	   DblRasterMx xCoordinate;
//	   DblRasterMx yCoordinate;
//	   DblRasterMx xCoordinate2;
//	   DblRasterMx yCoordinate2;
//	   DblRasterMx circle;
//	   DblRasterMx circle2;
//	   DblRasterMx distance;
//	   DblRasterMx distance2;
//	   DblRasterMx line1;
//	   DblRasterMx line2;
//	   DblRasterMx line3;
//	   DblRasterMx line4;
//	   DblRasterMx hollow;
//	   DblRasterMx fpAccflux;
//	   DblRasterMx fpSlope;
//	   DblRasterMx fpLongest;
//	   DblRasterMx fpYCoord;
//	   DblRasterMx fpelo;
//	   DblRasterMx fp2elo;
//	   DblRasterMx fp2Accflux;
//	   DblRasterMx fp2Slope;
//	   DblRasterMx fp2Longest;
//	   DblRasterMx fp2YCoord;
//	   DblRasterMx fp3Accflux;
//	   DblRasterMx fp3Slope;
//	   DblRasterMx fp3Longest;
//	   DblRasterMx upstreamSlope;
//	   DblRasterMx dSdL;
//	   DblRasterMx valve;
//	   DblRasterMx streampower;
//	   DblRasterMx downstreamDAdL;
//	   DblRasterMx downstreamsumdAdL;
//	   DblRasterMx streampowervalve;
//	   DblRasterMx dAdLA;
//	   DblRasterMx dSdLS;
//	   DblRasterMx fpdAdLA;
//	   DblRasterMx fpdSdLS;
//	   DblRasterMx fpShortest;
//	   DblRasterMx fp2dAdLA;
//	   DblRasterMx fp2dSdLS;
//	   DblRasterMx fp2Shortest;
//	   DblRasterMx nDiff;
//	   DblRasterMx crest;
//	   DblRasterMx mxShortest;
//	   DblRasterMx sumdAdL;
//	   DblRasterMx sumdSdL;
//	   DblRasterMx incision;
//
//	      
//
//	   mapattr(nSizeY,nSizeX,dx,500, mx0);
//	   mapattr(nSizeY,nSizeX,dx,0, mx3);
//	   mapattr(nSizeY,nSizeX,dx,0, mxSlopeCorrected);
//	   mapattr(nSizeY,nSizeX,dx,0, mxSlope);
//	   mapattr(nSizeY,nSizeX,dx,100, tst2);
//	   mapattr(nSizeY,nSizeX,dx,rand, tst3);
//	   mapattr(nSizeY,nSizeX,dx,1, one);
//	   mapattr(nSizeY,nSizeX,dx,-1, minusone);
//	   mapattr(nSizeY,nSizeX,dx,0, zero);
//	   mapattr(nSizeY,nSizeX,dx,50, fifty);
//	   mapattr(nSizeY,nSizeX,dx,10, ten);
//	   mapattr(nSizeY,nSizeX,dx,5, five);
//	   mapattr(nSizeY,nSizeX,dx,0.5, half);
//	   mapattr(nSizeY,nSizeX,dx,0.00001, zeroPlusABit);
//	   mapattr(nSizeY,nSizeX,dx,100, mxEdges);
//	   mapattr(nSizeY,nSizeX,dx,0.0, mxRandom);
//	   mapattr(nSizeY,nSizeX,dx,0.0, mxRandom2);
//	   //mapattr(nSizeY,nSizeX,dx,0.0, mxRandom2Max);
//	   mapattr(nSizeY,nSizeX,dx,0.0, mxDownstreamMax);
//	   mapattr(nSizeY,nSizeX,dx,0.0, mxUpstreamMin);
//	   mapattr(nSizeY,nSizeX,dx,0.0, mxEroded);
//	   mapattr(nSizeY,nSizeX,dx,0.0, sedOut);
//	   mapattr(nSizeY,nSizeX,dx,0.0, sedOutWithoutDiff);
//	   mapattr(nSizeY,nSizeX,dx,0.0, sedIn);
//	   mapattr(nSizeY,nSizeX,dx,0.0, change);
//	   //mapattr(nSizeY,nSizeX,dx,0.0, DiffSedOut);
//	   //mapattr(nSizeY,nSizeX,dx,0.0, DiffSedIn);
//	   mapattr(nSizeY,nSizeX,dx,0.0, lowerLimit);
//	   mapattr(nSizeY,nSizeX,dx,0.0, upperLimit);
//	   mapattr(nSizeY,nSizeX,dx,0, susp);					// set below!
//	   mapattr(nSizeY,nSizeX,dx,kTectDouble, boundDecrease);
//	   mapattr(nSizeY,nSizeX,dx,1, limitUpstream);
//	   mapattr(nSizeY,nSizeX,dx,1.5, m);
//	   mapattr(nSizeY,nSizeX,dx,1.5, n);
//	   mapattr(nSizeY,nSizeX,dx,kDiffDouble, kDiff);    //!
//	   mapattr(nSizeY,nSizeX,dx,kFluvDouble, kFluv);
//	   mapattr(nSizeY,nSizeX,dx,kTectDouble, kTect);
//       mapattr(nSizeY,nSizeX,dx,dx*dx, mxFluid);
//	   mapattr(nSizeY,nSizeX,dx,0, mxDiagonal);
//	   mapattr(nSizeY,nSizeX,dx,0, upstreamMxDiagonal);
//	   mapattr(nSizeY,nSizeX,dx,0, streams);
//	   mapattr(nSizeY,nSizeX,dx,0, streams2);
//	   mapattr(nSizeY,nSizeX,dx,0, streams3);
//	   mapattr(nSizeY,nSizeX,dx,0, streams4);
//	   mapattr(nSizeY,nSizeX,dx,0, mxUpstreamMax);
//	   mapattr(nSizeY,nSizeX,dx,0, dAdL);
//	   mapattr(nSizeY,nSizeX,dx,0, dAdL2);
//	   mapattr(nSizeY,nSizeX,dx,0, upstreamDAdL);
//	    mapattr(nSizeY,nSizeX,dx,0, upstreamsumdAdL);
//	   mapattr(nSizeY,nSizeX,dx,0, mxUpstreamTotal);	
//	   mapattr(nSizeY,nSizeX,dx,0, mxUpstreamRatio);
//	   mapattr(nSizeX,nSizeY,dx,0, perturbationsurface);	
//	   mapattr(nSizeX,nSizeY,dx,0, logMxLongest);
//	   mapattr(nSizeX,nSizeY,dx,0, mxLongest);
//	   mapattr(nSizeX,nSizeY,dx,0, logMxAccflux);
//	   mapattr(nSizeX,nSizeY,dx,0, elongation);
//	   mapattr(nSizeX,nSizeY,dx,0, suspSedIn);
//	   mapattr(nSizeX,nSizeY,dx,0, suspSedOut);
//	   mapattr(nSizeX,nSizeY,dx,0, suspSedOutLocal);
//	   mapattr(nSizeX,nSizeY,dx,0, change2);
//	   mapattr(nSizeY,nSizeX,dx,0, mxDownstreamMax);
//	   mapattr(nSizeY,nSizeX,dx,0, mxDownstreamRatio);
//	   mapattr(nSizeY,nSizeX,dx,0, points);
//	   mapattr(nSizeY,nSizeX,dx,0, points2);
//	   mapattr(nSizeY,nSizeX,dx,0, path1);
//	   mapattr(nSizeY,nSizeX,dx,0, path2);
//	   mapattr(nSizeY,nSizeX,dx,0, catchment1);
//	   mapattr(nSizeY,nSizeX,dx,0, catchment2);
//	   mapattr(nSizeY,nSizeX,dx,0, catchment3);
//	   mapattr(nSizeY,nSizeX,dx,0, mxAccflux);
//	   mapattr(nSizeY,nSizeX,dx,0, mxAccflux2);
//	   mapattr(nSizeY,nSizeX,dx,0, mxAccflux3);
//	   mapattr(nSizeY,nSizeX,dx,0, focusPoint);
//	   mapattr(nSizeY,nSizeX,dx,0, focusPoint2);
//	   mapattr(nSizeY,nSizeX,dx,0, focusPoint3);
//	   //mapattr(nSizeY,nSizeX,dx,0, yPositionFP);
//	   //mapattr(nSizeY,nSizeX,dx,0, zPositionFP);
//	   mapattr(nSizeY,nSizeX,dx,0, xCoordinate);
//	   mapattr(nSizeY,nSizeX,dx,0, yCoordinate);
//	   mapattr(nSizeY,nSizeX,dx,0, xCoordinate2);
//	   mapattr(nSizeY,nSizeX,dx,0, yCoordinate2);
//	   mapattr(nSizeY,nSizeX,dx,0, circle);
//	   mapattr(nSizeY,nSizeX,dx,0, circle2);
//	   mapattr(nSizeY,nSizeX,dx,0, distance);
//	   mapattr(nSizeY,nSizeX,dx,0, distance2);
//	   mapattr(nSizeY,nSizeX,dx,0, line1);
//	   mapattr(nSizeY,nSizeX,dx,0, line2);
//	   mapattr(nSizeY,nSizeX,dx,0, line3);
//	   mapattr(nSizeY,nSizeX,dx,0, line4);
//	   mapattr(nSizeY,nSizeX,dx,0, hollow);
//	   mapattr(nSizeY,nSizeX,dx,0, fpAccflux);
//	   mapattr(nSizeY,nSizeX,dx,0, fpSlope);
//	   mapattr(nSizeY,nSizeX,dx,0, fpLongest);
//	   mapattr(nSizeY,nSizeX,dx,0, fpYCoord);
//	   mapattr(nSizeY,nSizeX,dx,0, fpelo);
//	   mapattr(nSizeY,nSizeX,dx,0, fp2elo);
//	   mapattr(nSizeY,nSizeX,dx,0, fpShortest);
//	   mapattr(nSizeY,nSizeX,dx,0, fp2Shortest);
//	   mapattr(nSizeY,nSizeX,dx,0, fp2Accflux);
//	   mapattr(nSizeY,nSizeX,dx,0, fp2Slope);
//	   mapattr(nSizeY,nSizeX,dx,0, fp2Longest);
//	   mapattr(nSizeY,nSizeX,dx,0, fp2YCoord);
//	   mapattr(nSizeY,nSizeX,dx,0, fp3Accflux);
//	   mapattr(nSizeY,nSizeX,dx,0, fp3Slope);
//	   mapattr(nSizeY,nSizeX,dx,0, fp3Longest);
//	   mapattr(nSizeY,nSizeX,dx,0, upstreamSlope);
//	   mapattr(nSizeY,nSizeX,dx,0, dSdL);
//	   mapattr(nSizeY,nSizeX,dx,0, valve);
//	   mapattr(nSizeY,nSizeX,dx,0, streampower);
//	   mapattr(nSizeY,nSizeX,dx,0, downstreamDAdL);
//	   mapattr(nSizeY,nSizeX,dx,0, downstreamsumdAdL);
//	   mapattr(nSizeY,nSizeX,dx,0, streampowervalve);
//	   mapattr(nSizeY,nSizeX,dx,0, dAdLA);
//	   mapattr(nSizeY,nSizeX,dx,0, dSdLS);
//	   mapattr(nSizeY,nSizeX,dx,0, fpdAdLA);
//	   mapattr(nSizeY,nSizeX,dx,0, fpdSdLS);
//	   mapattr(nSizeY,nSizeX,dx,0, fp2dAdLA);
//	   mapattr(nSizeY,nSizeX,dx,0, fp2dSdLS);
//	   mapattr(nSizeY,nSizeX,dx,0, nDiff);
//	   mapattr(nSizeY,nSizeX,dx,0, crest);
//	   mapattr(nSizeY,nSizeX,dx,0, mxShortest);
//	   mapattr(nSizeY,nSizeX,dx,0, sumdAdL);
//	   mapattr(nSizeY,nSizeX,dx,0, sumdSdL);
//	   mapattr(nSizeY,nSizeX,dx,0, incision);
//	      
//
//
//	   for ( j = 0; j < nSizeX; j++ ){
//		   for ( l = 0; l < nSizeY; l++){
//			 if ( l==0 ){
//				  mxFluid(l,j) = dx*dx*1;
//			 } else {
//				//   if ( l > j - 20 | l > -j + 19 | j==20 | j==19 ){
//				//	   mxFluid(l,j) = 1;
//				//   } else {
//					   mxFluid(l,j) = dx*dx;
//				//   }
//			  }
//		   }
//	   }
//	   //tangent = U * 1;
//
//	   for ( j = 0; j < nSizeX; j++ ){
//						for ( l = 0; l < nSizeY; l++ ){
//							if (l > slopeBreak - 1){
//								mx0(l,j) = 0 + (nSizeY-l) * tangent * dx;
//							} else {
//								mx0(l,j) = (nSizeY - slopeBreak) * tangent * dx + (slopeBreak-l) * tangent * 0.5 * dx;
//							}
//						}
//			   }
//	   uniform(mxRandom);
//	   /*for ( j = 0; j < nSizeX; j++ ){
//		   for ( l = 0; l < nSizeY; l++ ){
//			   if (l < slopeBreak) {
//				   tst3(l,j) = 1 * tst3(l,j);
//			   }
//			   if (l > nSizeY-2){
//				   tst3(l,j) = 0;
//			   }
//			}
//	   }*/
//	   mx1 = mx0 + mxRandom * tst3 + tst2; //- tst3 * 0.5;
//   
//	   saveToAsc(mx1,0,"startTerrain");
//	 
// 
//	   loadFromAsc("d:\\pcrinst\\mx1200000.asc",mx1,10);
//	   //loadFromAsc("d:\\terRAIN\\test\\mx1020000.asc",mx1,10);
//	   //loadFromAsc("d:\\pcrinst\\borzsony50.asc",mx1,10);
//	   //printmx(mx3); 
//	   //mx1 = mx1 + tst2;
//	   mxEdges = mx1;
//
//
//					statFile.print("yPositionFP");
//					statFile.print("zPositionFP");
//					statFile.print("depthFP");
//					statFile.print("focusPointMigration");
//					statFile.print("pathsum");
//					statFile.print("areaCatchment3");
//					statFile.print("hollowShape");
//					statFile.print("catchmentShape");
//					statFile.print("widthRatio");
//					statFile.print("hollowWidth");
//					statFile.print("catchment1Width");
//					statFile.endl();
//	   
//	  
//	   for (i = 1; i <= iterationNumber; i++) {
//
//			   std::cout<<"Iteration nr: "<<i<<std::endl;
//			   
//			   
//			   /*if (( i % 1000 )==0){	
//					focusPointMigration = sqrt((yPositionFPOld - yPositionFP)*(yPositionFPOld - yPositionFP) + (zPositionFPOld - zPositionFP)*(zPositionFPOld - zPositionFP));
//					yPositionFPOld = yPositionFP;
//					zPositionFPOld = zPositionFP;
//				}*/
//			   
//					//statFile.print("szöveg");
//					// az elsõ pixel:
//					//statFile.print(mx1(49,0));
//					// a mátrix átlaga:
//					
//				if (( i % 1000 )==0){	
//					//double average = averageNotZeroItems(mx1); 
//					//statFile.print(average);
//					statFile.print(yPositionFP);
//					statFile.print(zPositionFP);
//					statFile.print(depthFP);
//					statFile.print(focusPointMigration);
//					statFile.print(pathsum);
//					statFile.print(areaCatchment3);
//					statFile.print(hollowShape);
//					statFile.print(catchmentShape);
//					statFile.print(widthRatio);
//					statFile.print(hollowWidth);
//					statFile.print(catchment1Width);
//					//statFile.print(yCoordCircleLimit);
//					//statFile.print(xCoordCircleMax);
//					//statFile.print(c1p1x);
//					//statFile.print(c1p2x);
//					//statFile.print(c1p3x);
//					//statFile.print(c1p4x);
//					// új sort kezd:
//					statFile.endl();
//				}
//
//			   pathsum=0;
//			   areaCatchment3=0;
//			   catchment1Width = 0;
//			   hollowWidth = 0;
//			   widthRatio = 0;
//			   hollowShape = 0;
//			   catchmentShape = 0;
//			   depthFP = 0;
//			   xCoordCircleMax = 0;
//			   yCoordCircleLimit = 0;
//			   yCoordCircleMin = (nSizeY-1)*dx;
//			    c1p1x = nSizeX*dx;				
//				c1p2x = 0;				
//				c1p3x = 0;				
//				c1p4x = 0;	
//				c1p5x = 0;
//				c1p6x = 0;
//				h1p1x = nSizeX * dx;
//				h1p2x = 0;
//
//			  		   
//		
//			   
//               slope(mx1,mxSlope); 
//			   max(mxSlope,zeroPlusABit, mxSlopeCorrected);
//			   lddcreate(mx1, mxLDD, true);
//			   multiflowLDD( 1, mx1, mxMLDD,true);
//			   //semiMultiflowLDD(mx1, mxSMLDD, true);
//			   diagonal(mxMLDD, mxDiagonal);
//			   accflux(mxMLDD,mxFluid,mxAccflux,accuFluxLimitRate);					  
//			   //longestflowpathlength(mxSMLDD, mxLongest);
//			   //max(mxLongest, five, mxLongest);
//			   //log10(mxLongest, logMxLongest);
//			   //log10(mxAccflux, logMxAccflux);
//			   //elongation = logMxLongest / logMxAccflux;	   
//			   //upstreammax( mxMLDD, mxAccflux, mxUpstreamMax, upstreamMaxAreaPosition, true);
//			   downstreammax( mxMLDD, mxAccflux, mxDownstreamMax, downstreamMaxAreaPosition, true);
//			   //upstreamtotal(mxMLDD, mxAccflux, mxUpstreamTotal, true,true);	
//			   //diagonal(upstreamMaxAreaPosition, upstreamMxDiagonal);
//			   //dAdL = (mxAccflux - mxUpstreamMax) * upstreamMxDiagonal;
//			   ////max(dAdL, fifty, dAdL);
//			   //getMatrixPosItems(mxSlopeCorrected,upstreamMaxAreaPosition,upstreamSlope);
//			   //dSdL = (mxSlopeCorrected - upstreamSlope) * upstreamMxDiagonal;
//			   //sum_of_upstreamdiffs(mxMLDD, mxAccflux, sumdAdL, 0.0);
//			   //sum_of_upstreamdiffs(mxMLDD, mxSlopeCorrected, sumdSdL, 0.0);
//			   //getMatrixPosItems(sumdAdL,upstreamMaxAreaPosition,upstreamsumdAdL);  
//			   ////dAdL2 = (dAdL - upstreamDAdL) * upstreamMxDiagonal; 	
//			   //dAdLA = m * sumdAdL / mxAccflux;
//			   //dSdLS = n * sumdSdL / mxSlopeCorrected;
//			   //nDiff = dSdLS / dAdLA;
//			   //valve = dAdLA + dSdLS;
//			   //streampower = mxAccflux^m * mxSlopeCorrected^n;
//			   //streampowervalve = streampower * valve;
//			   //getMatrixPosItems(sumdAdL,downstreamMaxAreaPosition,downstreamsumdAdL);
//			   // for ( j = 0; j < nSizeX; j++ ){
//						//for ( l = 1; l < nSizeY; l++ ){
//						//	crest(l,j) = 0;
//						//	if (mxAccflux (l,j) < 200){   
//						//		crest(l,j) = 1;
//						//	}
//						//}
//			   //}
//			   //spreadLDD(mxSMLDD,crest,mxShortest,0.0);
//			   //spreadLDDMax( mxSMLDD, crest, mxLongest,0.0 );
//			   //incision = mxLongest - mxShortest;
//
//			   //points2(nSizeY-1,10) = 1;
//			   //points2 = points2; //+ path1;
//			   //catchment(mxSMLDD,points2,catchment1);
//
//			   
//			      
//
//			   // identify streams via suspension ratios
//			   //for ( j = 0; j < nSizeX; j++ ){
//						//for ( l = 1; l < nSizeY; l++ ){
//						//	if (mxUpstreamTotal (l,j) == 0){   // | mxUpstreamMax(l,j) == mxUpstreamTotal(l,j) ){   // | mxAccflux(l,j) < 5
//						//		mxUpstreamRatio(l,j) = 0;
//						//	} else {
//						//		mxUpstreamRatio(l,j) = mxUpstreamMax(l,j) / mxUpstreamTotal(l,j);
//						//	}
//						//}
//			   //}
//			   for ( j = 0; j < nSizeX; j++ ){
//						for ( l = 0; l < nSizeY; l++ ){
//							mxDownstreamRatio(l,j) = mxDownstreamMax(l,j) / mxAccflux(l,j);
//							}
//			   }
//			   for ( j = 0; j < nSizeX; j++ ){
//						for ( l = 0; l < nSizeY; l++ ){
//							streams(l,j) = 0;
//							streams2(l,j) = 0;
//							}
//			   }
//			   for ( j = 0; j < nSizeX; j++ ){
//						for ( l = 0; l < nSizeY; l++ ){
//							susp(l,j) = 1;
//							}
//			   }
//			   //for ( j = 0; j < nSizeX; j++ ){
//						//for ( l = 0; l < nSizeY; l++ ){									
//						//	if ( mxDownstreamRatio(l,j) > 0.9999 && downstreamsumdAdL(l,j) < sumdAdL(l,j)){		// mxUpstreamRatio(l,j) > 0.1 &&						
//						//		streams(l,j) = 1; //0.5;                 						
//						//	}
//						//}
//			   //}			  
//			   //
//			   //path (mxSMLDD, streams, path1);
//			   //for ( j = 0; j < nSizeX; j++ ){			// because of the unincluded first pixel
//						//for ( l = 0; l < nSizeY; l++ ){			
//						//	if (path1(l,j) == 1 | streams(l,j) == 1){
//						//		path1(l,j) = 1;                 
//						//	}
//						//}
//			   //}
//
//			   for ( j = 0; j < nSizeX; j++ ){
//						for ( l = 0; l < nSizeY; l++ ){									
//							if (mxDownstreamRatio(l,j) > 0.9999 ){			//mxUpstreamRatio(l,j) > 0.1 && 					
//								streams2(l,j) = 1; //0.5;                 						
//							}
//						}
//			   }			  
//			   path (mxSMLDD, streams2, path2);
//			   for ( j = 0; j < nSizeX; j++ ){			// because of the unincluded first pixel
//						for ( l = 0; l < nSizeY; l++ ){			
//							if (path2(l,j) == 1 || streams2(l,j) == 1){
//								path2(l,j) = 1;                 
//							}
//						}
//			   }
//
//			   for ( j = 0; j < nSizeX; j++ ){		// set suspension
//						for ( l = 0; l < nSizeY; l++ ){			
//							if (path2(l,j) == 1){
//								susp(l,j) = 1.0;                 
//							} else {
//								susp(l,j) = 1;
//							}
//						}
//			   }
//			   ///*for ( j = 0; j < nSizeX; j++ ){
//						//for ( l = 0; l < nSizeY; l++ ){			
//						//	yCoordinate(l,j) = ((l+1) * dx) - (dx/2);                 
//						//}
//			   //}
//			   //for ( j = 0; j < nSizeX; j++ ){
//						//for ( l = 0; l < nSizeY; l++ ){			
//						//	xCoordinate(l,j) = ((j+1) * dx) - (dx/2);                 
//						//}
//			   //}*/
//			   //
//
//			   //accflux(mxLDD,path1,mxAccflux2);
//			   //accflux(mxLDD,path2,mxAccflux3);
//			   //for ( j = 0; j < nSizeX; j++ ){
//						//for ( l = 0; l < nSizeY; l++ ){			
//						//	focusPoint(l,j) = 0; 
//						//	focusPoint2(l,j) = 0; 
//						//	fpAccflux(l,j)=0;
//						//	fpSlope(l,j)=0;
//						//	fpLongest(l,j)=0;	
//						//	fpYCoord(l,j)=0;
//						//	fpdAdLA(l,j)=0;
//						//	fpdSdLS(l,j)=0;
//						//	fpelo(l,j)=0;
//						//	fpShortest(l,j)=0;
//						//	fp2Accflux(l,j)=0;
//						//	fp2Slope(l,j)=0;
//						//	fp2Longest(l,j)=0;	
//						//	fp2YCoord(l,j)=0;
//						//	fp3Accflux(l,j)=0;
//						//	fp3Slope(l,j)=0;
//						//	fp3Longest(l,j)=0;	
//						//}
//			   //}
//			   //for ( j = 0; j < nSizeX; j++ ){
//						//for ( l = 0; l < nSizeY; l++ ){			
//						//	//if (mxAccflux2(l,j) == 1){
//						//	if (DoubleUtil::cmpD6(mxAccflux2(l,j),1.0)){
//						//		focusPoint(l,j) = 1; 
//						//		//yPositionFP = yCoordinate(l,j) / yCoordinate(nSizeY-1,j);  //nSizeY;
//						//		//zPositionFP = mx1(l,j)/(mx1(0,1)-mx1(nSizeY,1));
//						//		//xCoordFP = (j+1)*dx - dx/2;
//						//		//yCoordFP = (l+1)*dx - dx/2;
//						//		fpAccflux(l,j)=mxAccflux(l,j);
//						//		fpSlope(l,j)=mxSlope(l,j);
//						//		fpLongest(l,j)=mxLongest(l,j);	
//						//	    fpdAdLA(l,j)=dAdLA(l,j);
//						//		fpdSdLS(l,j)=dSdLS(l,j);
//						//		fpelo(l,j)=elongation(l,j);
//						//		fpShortest(l,j)=mxShortest(l,j);
//						//		//fpYCoord(l,j)=yCoordinate(l,j);	*/
//						//	} 
//						//	//if (DoubleUtil::cmpD6(mxAccflux2(l,j),2.0)){
//						//	if (DoubleUtil::cmpD6(mxAccflux3(l,j),1.0)){
//						//		focusPoint2(l,j) = 1;
//						//		fp2Accflux(l,j)=mxAccflux(l,j);
//						//		fp2Slope(l,j)=mxSlope(l,j);
//						//		fp2Longest(l,j)=mxLongest(l,j);	
//						//		fp2dAdLA(l,j)=dAdLA(l,j);
//						//		fp2dSdLS(l,j)=dSdLS(l,j);
//						//		fp2elo(l,j)=elongation(l,j);
//						//		fp2Shortest(l,j)=mxShortest(l,j);
//						//		//fp2YCoord(l,j)=yCoordinate(l,j);	*/
//						//	} 
//						//	//if (DoubleUtil::cmpD6(mxAccflux2(l,j),2.0)){
//						//	//	focusPoint3(l,j) = 1;
//						//	//	fp3Accflux(l,j)=mxAccflux(l,j);
//						//	//	fp3Slope(l,j)=mxSlope(l,j);
//						//	//	fp3Longest(l,j)=mxLongest(l,j);	
//						//	//	//fp3Coord(l,j)=yCoordinate(l,j);	*/
//						//	//} 
//						//}
//			   //}		
//			    						
//			    //sedOut=kFluv*tst2*tst3;			   	
//			   sedOut = kDiff*mxSlopeCorrected + kFluv*(mxSlopeCorrected^n)*(mxAccflux^m);
//			   /*sedOutWithoutDiff = kFluv*(mxSlopeCorrected^n)*(mxAccflux^m);
//			   for ( j = 0; j < nSizeX; j++ ){
//						for ( l = 0; l < nSizeY; l++ ){
//							if (streams(l,j) == 1){
//								sedOut(l,j) = sedOutWithoutDiff(l,j); 	
//							} else {
//								sedOut(l,j) = sedOut(l,j); 
//							}
//						}
//			   }*/
//    		 
//			  
//			   upstreamtotal(mxMLDD, sedOut, sedIn, true,true);				 
//			   change = sedIn * susp - sedOut * mxDiagonal;		   
//			   //change = minusone * kFluv * streampowervalve - kDiff * mxSlopeCorrected;
//			   //erosion=sedOut-sedIn*susp;
//			   //downstreammax( mxMLDD, erosion, erosionDownstreamMax);
//			   //erosionDifference=erosion-erosionDownstreamMax;
//			   //heightDifference=mx1-mxDownstreamMax;
//			   //mx1 = mx1 - sedOut + sedIn*susp;
//			   //upperLimit = mx1+limitUpstream;
//			   //lowerLimit = mx1-limitUpstream;
//			   mx1 = mx1 + change;
//			   //min( mx1, upperLimit, mx1);
//			   //max( mx1, lowerLimit, mx1); 
//
//
//				// clean edges
//			  /* for ( j = 0; j < nSizeY; j++ ){
//                       valve(j,nSizeX-1) = zero(j,nSizeX-1);
//                       valve(j,0) = zero(j,0);
//               }
//               for ( j = 1; j < nSizeX-1; j++ ){
//                       valve(nSizeY-1,j) = zero(nSizeY-1,j);
//                       valve(0,j) = zero(0,j);
//               }
//			   for ( j = 0; j < nSizeY; j++ ){
//                       change(j,nSizeX-1) = -kTect(j,nSizeX-1);
//                       change(j,0) = -kTect(j,0);
//               }
//               for ( j = 1; j < nSizeX-1; j++ ){
//                       change(nSizeY-1,j) = -kTect(nSizeY-1,j);
//                       change(0,j) = -kTect(0,j);
//               }
//			   for ( j = 0; j < nSizeY; j++ ){
//                       dSdL(j,nSizeX-1) = zero(j,nSizeX-1);
//                       dSdL(j,0) = zero(j,0);
//               }
//               for ( j = 1; j < nSizeX-1; j++ ){
//                       dSdL(nSizeY-1,j) = zero(nSizeY-1,j);
//                       dSdL(0,j) = zero(0,j);
//               }
//			   for ( j = 0; j < nSizeY; j++ ){
//                       streampowervalve(j,nSizeX-1) = zero(j,nSizeX-1);
//                       streampowervalve(j,0) = zero(j,0);
//               }
//               for ( j = 1; j < nSizeX-1; j++ ){
//                       streampowervalve(nSizeY-1,j) = zero(nSizeY-1,j);
//                       streampowervalve(0,j) = zero(0,j);
//               }
//			   for ( j = 0; j < nSizeY; j++ ){
//                       dSdLS(j,nSizeX-1) = zero(j,nSizeX-1);
//                       dSdLS(j,0) = zero(j,0);
//               }
//               for ( j = 1; j < nSizeX-1; j++ ){
//                       dSdLS(nSizeY-1,j) = zero(nSizeY-1,j);
//                       dSdLS(0,j) = zero(0,j);
//               }
//			    for ( j = 0; j < nSizeY; j++ ){
//                       nDiff(j,nSizeX-1) = zero(j,nSizeX-1);
//                       nDiff(j,0) = zero(j,0);
//               }
//               for ( j = 1; j < nSizeX-1; j++ ){
//                       nDiff(nSizeY-1,j) = zero(nSizeY-1,j);
//                       nDiff(0,j) = zero(0,j);
//               }*/
//
//			   //for ( j = 0; j < nSizeX; j++ ){
//						//for ( l = 0; l < nSizeY; l++ ){			
//						//	//if (catchment1(l,j) == 1){ 
//						//	if (mxLDD(l,j) == 5) {
//						//	   mx1(l,j) = mx1(l,j) + sedIn(l,j) * susp(l,j); // - kTect(l,j);   // reflecting boundary
//						//   } else {
//						//		mx1(l,j) = mx1(l,j) + change(l,j); // + kTect(l,j);
//						//   }							
//						//}
//			   //}
//
//			   //if (i < 2) {
//				   mxEdges = mxEdges-kTect;
//			   //}
//			   ////// lower Edges - infinite sink
//			   //for ( j = 0; j < nSizeY; j++ ){
//      //                 mx1(j,nSizeX-1) = mxEdges(j,nSizeX-1);
//					 //  //mx1(j,nSizeX-2) = mxEdges(j,nSizeX-2);
//      //                 mx1(j,0) = mxEdges(j,0);
//					 //  //mx1(j,1) = mxEdges(j,1);
//      //         }
//
//      //         for ( j = 1; j < nSizeX-1; j++ ){
//      //                 mx1(nSizeY-1,j) = mxEdges(nSizeY-1,j);
//					 //  //mx1(nSizeY-2,j) = mxEdges(nSizeY-2,j);
//      //                 mx1(0,j) = mxEdges(0,j);
//					 //  //mx1(1,j) = mxEdges(1,j);
//      //         }
//			   
//			  ////  //lower bottomSide
//			  for ( j = 0; j < nSizeX; j++ ){
//                       mx1(nSizeY-1,j)= mxEdges(nSizeY-1,j);
//					   //mx1(nSizeY-1,j)= mx1(nSizeY-1,j)-boundDecrease(nSizeY-1,j);
//               }
//			   
//				   //lower top and middleofbottom
//				 /*    for ( j = 0; j < nSizeX; j++ ){
//                       mx1(0,j)= mxEdges(0,j);
//					 }
//					  *///mx1(nSizeY-1,30) = mxEdges(nSizeY-1,30);
//
//
//			     //lower bottomSide and topSide
//			  //for ( j = 0; j < nSizeX; j++ ){
//     //                  mx1(nSizeY-1,j)= mxEdges(nSizeY-1,j);
//					//   mx1(0,j)= mxEdges(0,j);
//					//   //mx1(nSizeY-1,j)= mx1(nSizeY-1,j)-boundDecrease(nSizeY-1,j);
//     //          }
//
//			   //// lower bottomPlain
//			   //for ( j = 0; j < nSizeX; j++ ){
//				  // for ( l = nSizeY-1; l > nSizeY-1-20; l-- ){
//					 //  //if (l > nSizeY-1-1){
//						//   //mx1(l,j)= mxEdges(l,j); //mx1(l,j)- boundDecrease(l,j);
//					 //  //} else {
//						//   mx1(l,j)= mx1(l,j)-kTect(l,j);
//						//   //mx1(nSizeY-1,j) = mx1(l,j);
//					 //  //}					   		   
//				  // }
//      //         }
//
//
//			   // lower bottomLeft
//			   //mx1(nSizeY-1,0) = mxEdges(nSizeY-1,0);
//
//			 if (( i % 1000 )==0){
//					saveToAsc(mx1,i,"mx2",true);
//					saveToAsc(mx1,i,"mx1");
//					saveToAsc(mxAccflux,i,"accflux",true);
//					saveToAsc(mxSlopeCorrected,i,"slope",true);
//					saveToAsc(sedOut, i, "sedOut");
//					saveToAsc(sedIn, i, "sedIn");
//					saveToAsc(change, i, "change",true);
//					saveToAsc(mxLDD, i, "mxLDD");
//					saveToAsc(mxMLDD, i, "mxMLDD");
//					saveToAsc(mxDiagonal, i, "mxDiagonal");
//					saveToAsc(streams, i, "streams");
//					saveToAsc(susp, i, "susp");
//					saveToAsc(mxUpstreamMax, i, "mxUpstreamMax");
//					saveToAsc(crest, i, "crest",true);
//					saveToAsc(mxUpstreamRatio, i, "mxUpstreamRatio");
//					saveToAsc(upstreamMaxAreaPosition,i,"upstreamMaxAreaPosition");
//					saveToAsc(mxDiagonal, i, "mxDiagonal");
//					saveToAsc(suspSedOut, i, "suspSedOut");
//					saveToAsc(suspSedIn, i, "suspSedIn");
//					saveToAsc(suspSedOutLocal, i, "suspSedOutLocal");
//					saveToAsc(change2, i, "change2");
//					saveToAsc(mxFluid, i, "mxFluid");
//					saveToAsc(mxDownstreamMax, i, "mxDownstreamMax");
//					saveToAsc(mxDownstreamRatio, i, "mxDownstreamRatio");
//					saveToAsc(points, i, "points");
//					saveToAsc(points2, i, "points2");
//					saveToAsc(path1, i, "path1",true);
//					saveToAsc(path2, i, "path2");
//					saveToAsc(catchment1, i, "catchment1");
//					saveToAsc(catchment3, i, "catchment3");
//					saveToAsc(streams2, i, "streams2");
//					saveToAsc(downstreamDAdL, i, "downstreamDAdL");
//					saveToAsc(dAdL2, i, "dAdL2");
//					saveToAsc(focusPoint, i, "focusPoint");
//					saveToAsc(focusPoint2, i, "focusPoint2");
//					saveToAsc(focusPoint3, i, "focusPoint3");
//					saveToAsc(catchment2, i, "catchment2");
//					saveToAsc(yCoordinate, i, "yCoordinate");
//					saveToAsc(xCoordinate, i, "xCoordinate");
//					saveToAsc(circle2, i, "circle2");
//					saveToAsc(distance, i, "distance");
//					saveToAsc(distance2, i, "distance2");
//					saveToAsc(line1, i, "line1");
//					saveToAsc(hollow, i, "hollow");
//					saveToAsc(fpAccflux, i, "fpAccflux",true);
//					saveToAsc(fpSlope, i, "fpSlope",true);
//					saveToAsc(fpLongest, i, "fpLongest",true);
//					saveToAsc(fpYCoord, i, "fpYCoord");
//					saveToAsc(fpelo, i, "fpelo",true);
//					saveToAsc(fpShortest, i, "fpShortest",true);
//					saveToAsc(fpelo, i, "fp2elo",true);
//					saveToAsc(fpShortest, i, "fp2Shortest",true);
//					saveToAsc(fp2Accflux, i, "fp2Accflux");
//					saveToAsc(fp2Slope, i, "fp2Slope");
//					saveToAsc(fp2Longest, i, "fp2Longest");
//					saveToAsc(fp2YCoord, i, "fp2YCoord");
//					saveToAsc(fp3Accflux, i, "fp3Accflux");
//					saveToAsc(fp3Slope, i, "fp3Slope");
//					saveToAsc(fp3Longest, i, "fp3Longest");
//					saveToAsc(dAdL, i, "dAdL",true);
//					saveToAsc(upstreamDAdL, i, "upstreamDAdL");
//					saveToAsc(dAdL2, i, "dAdL2");
//					saveToAsc(upstreamSlope, i,"upstreamSlope");
//					saveToAsc(dSdL, i, "dSdL",true);
//					saveToAsc(valve, i, "valve",true);
//				    saveToAsc(streampower, i, "streampower",true);
//					saveToAsc(streampowervalve, i, "streampowervalve",true);
//					saveToAsc(dAdLA, i, "dAdLA",true);
//					saveToAsc(dSdLS, i, "dSdLS",true);
//				    saveToAsc(fpdAdLA, i, "fpdAdLA",true);
//					saveToAsc(fpdSdLS, i, "fpdSdLS",true);
//					saveToAsc(fpdAdLA, i, "fp2dAdLA",true);
//					saveToAsc(fpdSdLS, i, "fp2dSdLS",true);
//					saveToAsc(nDiff, i, "nDiff",true);
//					saveToAsc(mxLongest, i, "mxLongest",true);
//					saveToAsc(mxShortest, i, "mxShortest",true);
//					saveToAsc(sumdAdL, i, "sumdAdL",true);
//					saveToAsc(sumdSdL, i, "sumdSdL",true);
//					saveToAsc(elongation, i, "elongation",true);
//					saveToAsc(incision, i, "incision",true);
//			   }               
//       }
//
//	   saveToAsc(mx1,0,"result");
//	   printmx(mx1);
//
//
//	
//	return true;
//}
//
//};