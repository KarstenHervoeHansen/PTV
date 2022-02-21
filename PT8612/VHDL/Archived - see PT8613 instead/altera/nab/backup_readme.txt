backup readme file 
	( alle ændringer fra backup til backup version er kun foretaget i 
	video_sm og tilhørende underkomponenter color_lut og state_counter

bu1 = 	udgangspunkt med een generator ( HD only ) implementeret,

bu2 = 	color_lut.vhd rettet til ( pattern 000, 101 og 110 implementeret - 1080 lines only )

bu3 = 	alle testpatterns i alle hdformater implementeret vertikalt i video_sm.vhd
	( video_vertical_state processen )

bu4 = 	alle testpatterns implementeret	vertikalt i alle hdformater og horisontalt i 1080 liniers formater.
	( undtagen smpte colorbar horisontal state v2 )

bu5 = 	alle testpatterns implementeret	vertikalt i alle hdformater og horisontalt i 1080 liniers formater.

bu6 = 	alle testpatterns implementeret	vertikalt og horisontalt i alle hdformater.

bu7 = 	color_lut.vhd ændret ( fejl rettet i rp219 picture )
	sidste backup inden den serielle kommunikation implementeres

bu8 =	seriel kommunikation implementeret. Patterns kan ændres via PT5300 ( ikke format ).

bu9 =	HD-format skift implementeret. Der er et problem med 720P-50,30,25,24 og 23.98 
	( det aktive video område starter ca. 150 pixel inde i billedet - det KAN være AJA konverteren,
	der laver ballade ) Derfor er der lavet en workaround, der holder format på 720P/60, hvis der vælges
	et af ovenstående.

bu10 =	25% grey ændret til 75% white i 75% colorbar w. grey signalet.