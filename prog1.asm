
//
// Un exemple de boucle avec
// évolution de R1 par incrément de 2000.
//

	define INCR 2000
	define SYSC_EXIT 100
	define SYSC_PUTI 200

	set R1, 0					// R1 = 0
	set R2, INCR				// R2 = INCR
	set R3, 6000				// R3 = 6000
loop:							// définir loop
	ifgt R1, R3, end			// si (R1 > R3) aller à end
	//load R1, R3, 0				// Erreur surcharge 
	add R1, R2, 0				// R1 = R1 + R2 + 0
	nop							// ne rien faire
	jump loop					// aller à loop
end:
	//halt						// poweroff
	sysc R0, R0, SYSC_PUTI     // Appel au système pour SYSC_EXIT
