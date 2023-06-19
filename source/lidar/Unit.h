/*-----------------------------------------------------------------------------------*/
/* FILE	        : UNIT.H                                                             */
/* MODULE       : UNIT                                                               */
/* AUTHOR       : HGU SASLAB                                                         */
/* DATE			: 2016. 2.29.												         */
/*-----------------------------------------------------------------------------------*/


#define UNIT_PI			        (double)( 3.141592653589793	  ) 
#define UNIT_GRAV		        (double)( 9.81				  )
#define UNIT_MACH		        (double)( 340.29			  )

#define UNIT_RAD2DEG	        (double)( 180.0/UNIT_PI		  )
#define UNIT_DEG2RAD	        (double)( UNIT_PI/180.0		  )
#define UNIT_DEG2MDEG			(double)( 1000.0              )

#define UNIT_ARCMIN				(double)( 1.0/60.0			  )

#define UNIT_HZ2RAD 	        (double)( 2.0*UNIT_PI		  )
#define UNIT_RAD2HZ		        (double)( 1.0/UNIT_HZ2RAD	  )

#define UNIT_Km2Meter	        (double)( 1000.0   		      )
#define UNIT_Meter2Km	        (double)( 0.001				  )

#define UNIT_Meter2Feet	        (double)( 3.2808   			  )
#define UNIT_Feet2Meter	        (double)( 1.0/UNIT_Meter2Feet )

#define UNIT_Inch2Meter	        (double)( 0.0254   			  )
#define UNIT_Meter2Inch	        (double)( 1.0/UNIT_Inch2Meter )

#define UNIT_Lbf2N	            (double)( 4.448221   		  )
#define UNIT_N2Lbf	            (double)( 1.0/UNIT_Lbf2N      )

#define UNIT_Ton2Kg				(double)( 1000.0              )

#define UNIT_kN2N				(double)( 1000.0              )

#define UNIT_Kgf2N	            (double)( 9.806650   		  )
#define UNIT_N2Kgf	            (double)( 1.0/UNIT_Kgf2N      )

#define UNIT_MILLI				(double)( 0.001               )
#define UNIT_MICRO				(double)( 0.000001            )

#define UNIT_Hr2Sec				(double)( 3600.0			  )
#define UNIT_Sec2Ms				(double)( 1000.0			  )
#define UNIT_Ms2Sec				(double)( 0.001				  )

#define UNIT_NUM2PCT            (double)( 100.0               )
#define UNIT_PCT2NUM            (double)( 0.01                )

#define UNIT_Meter2Mm           (double)( 1000.0              )
#define UNIT_Mm2Meter           (double)( 0.001               )