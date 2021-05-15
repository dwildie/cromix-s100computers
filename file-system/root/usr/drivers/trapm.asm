;
;
;
	entry trapm

	psect   trapm(rea,exe)
        
trapm:  
	trap    #14    
	rts

	end
