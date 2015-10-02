/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2012 CERN (www.cern.ch)
 * Author: Grzegorz Daniluk <grzegorz.daniluk@cern.ch>
 * Author: Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
/*  Command: sfp
    Arguments: subcommand [subcommand-specific args]

    Description: SFP detection/database manipulation.

		Subcommands:
			add vendor_type delta_tx delta_rx alpha - adds an SFP to the database, with given alpha/delta_rx/delta_rx values
			show - shows the SFP database
      match - tries to get calibration parameters from DB for a detected SFP
      erase - cleans the SFP database
			detect - detects the transceiver type
*/

#include <string.h>
#include <stdlib.h>
#include <sys.h>

#include "shell.h"
#include "eeprom.h"
#include "syscon.h"

#include "sfp.h"

static int cmd_sfp(const char *args[])
{
	int8_t sfpcount = 1, i, temp;
	int8_t sfp_detected[2] = { 0 , 0 };

	char *iface_port;

	struct s_sfpinfo sfp;
	static char pn[SFP_PORTS][SFP_PN_LEN + 1] = {"\0"};



	if (args[0] && !strcasecmp(args[0], "detect")) {                          //DETECT

		if (!sfp_present(0)) {
			mprintf("No SFP 1.\n");
		} else {
			sfp_read_part_id(pn[0], 0);
			sfp_detected[0] = 1;
			mprintf("SFP 1: %s\n", pn[0]);
		}

		if (!sfp_present(1)) {
			mprintf("No SFP 2.\n");
		} else {
			sfp_read_part_id(pn[1], 1);
			sfp_detected[1]=  1;
			mprintf("SFP 2: %s\n", pn[1]);
		}

		pn[0][16] = 0;
		pn[1][16] = 0;

		return 0;
	}
	else if (!strcasecmp(args[0], "erase")) {                                //ERASE
		if (eeprom_sfpdb_erase(FMC_I2C, FMC_EEPROM_ADR) == EE_RET_I2CERR)
			mprintf("Could not erase DB\n");

	} else if (args[5] && !strcasecmp(args[0], "add")) {                     //ADD
		if (strlen(args[1]) > 16)
			temp = 16;
		else
			temp = strlen(args[1]);
		for (i = 0; i < temp; ++i)
			sfp.pn[i] = args[1][i];
		while (i < 16)
			sfp.pn[i++] = ' ';	//padding

		iface_port = (char *) args[2];		//port name

		sfp.port_position = atoi(&iface_port[2]);	// port index

		sfp.dTx = atoi(args[3]);
		sfp.dRx = atoi(args[4]);
		sfp.alpha = atoi(args[5]);

		temp = eeprom_get_sfp(FMC_I2C, FMC_EEPROM_ADR, &sfp, 1, 0);
		if (temp == EE_RET_DBFULL)
			mprintf("SFP DB is full\n");
		else if (temp == EE_RET_I2CERR)
			mprintf("I2C error\n");
		else
			mprintf("%d SFPs in DB\n", temp);

	} else if (args[0] && !strcasecmp(args[0], "show")) {                     //SHOW
		for (i = 0; i < sfpcount; ++i) {

		temp = eeprom_get_sfp(FMC_I2C, FMC_EEPROM_ADR, &sfp, 0, i);
			if (!i) {
				sfpcount = temp;	//only in first round valid sfpcount is returned from eeprom_get_sfp

				mprintf("SFP count is %i \n", sfpcount);

				if (sfpcount == 0 || sfpcount == 0xFF) {
					mprintf("SFP database empty...\n");
					return 0;
				} else if (sfpcount == -1) {
					mprintf("SFP database corrupted...\n");
					return 0;
				}
			}
			mprintf("%d: PN:", i + 1);
			for (temp = 0; temp < 16; ++temp)
				mprintf("%c", sfp.pn[temp]);

			mprintf(" wr%i  => dTx: %d, dRx: %d, alpha: %d\n",sfp.port_position, sfp.dTx, sfp.dRx, sfp.alpha);
		}

	} else if (args[0] && !strcasecmp(args[0], "match")) {                     //MATCH
		if ((pn[0][0] == '\0') && (pn[1][0] == '\0')) {
			mprintf("Run sfp detect first\n");
			return 0;
		}

		if( sfp_present(0) ) {
			//Matching SFP to port1.
			strncpy(sfp.pn, pn[0], SFP_PN_LEN);
			if (eeprom_match_sfp(FMC_I2C, FMC_EEPROM_ADR, &sfp, 0) > 0) {
				mprintf("SFP 1 matched, dTx=%d, dRx=%d, alpha=%d\n", sfp.dTx, sfp.dRx, sfp.alpha);
				sfp_deltaTx[0] = sfp.dTx;
				sfp_deltaRx[0] = sfp.dRx;
				sfp_alpha[0] = sfp.alpha;
			} else
				mprintf("Could not match SFP 1 to DB\n");
		}

		if( sfp_present(1) ) {
			//Matching SFP to port1.
			strncpy(sfp.pn, pn[1], SFP_PN_LEN);
			if (eeprom_match_sfp(FMC_I2C, FMC_EEPROM_ADR, &sfp, 1) > 0) {
				mprintf("SFP 2 matched, dTx=%d, dRx=%d, alpha=%d\n", sfp.dTx, sfp.dRx, sfp.alpha);
				sfp_deltaTx[1] = sfp.dTx;
				sfp_deltaRx[1] = sfp.dRx;
				sfp_alpha[1] = sfp.alpha;
			} else
				mprintf("Could not match SFP 2 to DB\n");
		}

		return 0;
	}

	return 0;
}

DEFINE_COMMAND(sfp) = {
	.name = "sfp",
	.exec = cmd_sfp,
};
