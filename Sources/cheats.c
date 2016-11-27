#include "cheats.h"
#include "hid.h"


char	*builder_name = "AnalogMan";
u8      matchingHair;

/********************************
*				*
*	  EXP Modifiers		*
*				*
********************************/

// Sets the stage for Experience Modifier code
void	exp_x()
{
	WRITEU32(0x595800, 0xE1D002B2);
	WRITEU32(0x595804, 0xE92D4002);
	WRITEU32(0x595808, 0xE3A01001);
	WRITEU32(0x59580C, 0xE0000190);
	WRITEU32(0x595810, 0xE8BD8002);
	WRITEU32(0x48F1EC, 0xEB041983);	
}	

// Reads current EXP modifier and prints it to the menu
void	updateEXP(void)
{
	u8 exp_rate = READU8(0x595808);
	xsprintf(currentEXP, "Current EXP rate: %dx", exp_rate);
}

// Increases EXP modifier by 1
void	increaseEXP(void)
{
	u8 exp_rate = READU8(0x595808);
	if (exp_rate < 0xFF)
	{
		exp_rate++;
		WRITEU8(0x595808, exp_rate);
		updateEXP();
	}
	disableCheat(i_increaseEXP);
}

// Increases EXP modifier by 10
void	increaseEXP10(void)
{
	u8 exp_rate = READU8(0x595808);
	if (exp_rate < 0xF5)
	{
		exp_rate += 10;
		WRITEU8(0x595808, exp_rate);
		updateEXP();
	}
	disableCheat(i_increaseEXP10);
}

// Decrease EXP modifier by 1
void	decreaseEXP(void)
{
	u8 exp_rate = READU8(0x595808);
	if (exp_rate > 0x00)
	{
		exp_rate--;
		WRITEU8(0x595808, exp_rate);
		updateEXP();
	}
	disableCheat(i_decreaseEXP);
}

// Decrease EXP modifier by 10
void	decreaseEXP10(void)
{
	u8 exp_rate = READU8(0x595808);
	if (exp_rate > 0x0A)
	{
		exp_rate -= 10;
		WRITEU8(0x595808, exp_rate);
		updateEXP();
	}
	disableCheat(i_decreaseEXP10);
}

// Resets EXP modifier back to 1x
void	resetEXP(void)
{
	WRITEU8(0x595808, 0x1);
	updateEXP();
	disableCheat(i_resetEXP);
}


/********************************
*				*
*	Movement Modifiers	*
*				*
********************************/

// Increases run speed to 1.375x
void	runFaster(void)
{
	if (!is_pressed(BUTTON_B))
		WRITEU32(0x39AF74, 0x3F800000);
	else
		WRITEU32(0x39AF74, 0x3FB00000);
}

// Disables occational tripping animation
void	neverTrip(void)
{
	WRITEU8(0x3419833E, 0x0);
}

// Walk through wall while R is held down
void	walkThruWalls(void)
{
	if (!is_pressed(BUTTON_R))
	{
		WRITEU32(0x0039D140, 0xEB01E7E7);
		WRITEU32(0x0039D274, 0xEB01E79A);
	} else {
		WRITEU32(0x0039D140, 0xE1A00000);
		WRITEU32(0x0039D274, 0xE1A00000);
	}
}


/********************************
*				*
*	Pokemon Modifiers	*
*				*
********************************/

// 100% Catch rate for Pokemon
void	catch100(void)
{
	WRITEU32(0x59585C, 0xE5D00008);
	WRITEU32(0x595860, 0xE92D4003);
	WRITEU32(0x595864, 0xE59D0010);
	WRITEU32(0x595868, 0xE59F100C);
	WRITEU32(0x59586C, 0xE1510000);
	WRITEU32(0x595870, 0x24000F8);
	WRITEU32(0x595874, 0x58D0010);
	WRITEU32(0x595878, 0xE8BD8003);
	WRITEU32(0x59587C, 0x6D839C);
	WRITEU32(0x48F1E0, 0xEB04199D);
}

// Make wild Pokemon shiny. Activate with START+L and deactivate with START+R
void	shinyPokemon(void)
{
	if (is_pressed(BUTTON_ST + BUTTON_L))
		WRITEU32(0x3183EC, 0xEA00001C);
	
	if (is_pressed(BUTTON_ST + BUTTON_R))
		WRITEU32(0x3183EC, 0x0A00001C);
}

// Rename any Pokemon
void	renamePokemon(void)
{
	WRITEU32(0x4A84F8, 0xE3A00001);
}

// Egg instantly ready when talking to Nursery NPC while holding START
void	instantEgg(void)
{
	if (!is_pressed(BUTTON_ST))
	{
		WRITEU32(0x444A6C, 0xE2800E1E);
		WRITEU32(0x444A70, 0xE1D000D0);
		WRITEU32(0x444A74, 0xE12FFF1E);
	} else {
		WRITEU32(0x444A6C, 0xE3A01001);
		WRITEU32(0x444A70, 0xE5C011E0);
		WRITEU32(0x444A74, 0xEA00684B);
	}
}

// Instant egg hatching in one step
void	instantHatch(void)
{
	WRITEU32(0x5958C0, 0xE59D000C);
	WRITEU32(0x5958C4, 0xE59F500C);
	WRITEU32(0x5958C8, 0xE1500005);
	WRITEU32(0x5958CC, 0x3A00000);
	WRITEU32(0x5958D0, 0x11A00004);
	WRITEU32(0x5958D4, 0xE12FFF1E);
	WRITEU32(0x5958D8, 0x6CE724);
	WRITEU32(0x4919E0, 0xEB040FB6);
}


/********************************
*				*
*	     QR Codes		*
*				*
********************************/

// Removed 24 hour wait time for island scanning
void	removeIslandScanWait(void)
{
	WRITEU32(0x43DAA8, 0xE3A00000);
}

// Sets QR Scan points to 100 allowing you to Island Scan
void	qrScan100(void)
{
	if (READU32(0x67206C) != 0x0)
	{
		u32 offset;
		
		offset = READU32(0x4 + READU32(0x24 + READU32(0x67206C)));
		
		WRITEU8(0x6960B + offset, 0x64);
	}
}


/********************************
*				*
*     Currency Manipulators	*
*				*
********************************/

// Set PokeDollars to 9,999,999
void	maxMoney(void)
{
	WRITEU32(0x330D8FC0, 0x98967F);
}

// Set current Festival Coins to 999,999
void	maxCoins(void)
{
	WRITEU32(0x33124D58, 0xF423F);
}

// Set total Festival Coins to 9,999,999
void	totalCoins(void)
{
	WRITEU32(0x33124D5C, 0x98967F);
}

// Set total Thumbs Up for photos to 1,500,000
void	totalThumbs(void)
{
	WRITEU32(0x33138B8C, 0x16E360);
}

// Set Battle Points to 9,999
void	maxBP(void)
{
	if (READU32(0x67206C) != 0x0)
	{
		u32 offset;
		
		offset = READU32(0x4 + READU32(0x24 + READU32(0x67206C)));
		
		WRITEU16(0x37B0 + offset, 0x270F);
	}
}


/********************************
*				*
*	      Items		*
*				*
********************************/

// Gives all items and sets them to quantity 950 (credit to Nanquitas for method)
void    allItems(void)
{
    u32    data;
    u32    offset;
    u32    address;
    
    data = 0;
    offset = 0;
    address = 0;
    
    offset = 0x330D5934;
    static const u8    buffer[] =
    {
        0x01, 0xD8, 0x0E, 0x00, 0x02, 0xD8, 0x0E, 0x00,
        0x03, 0xD8, 0x0E, 0x00, 0x04, 0xD8, 0x0E, 0x00,
        0x05, 0xD8, 0x0E, 0x00, 0x06, 0xD8, 0x0E, 0x00,
        0x07, 0xD8, 0x0E, 0x00, 0x08, 0xD8, 0x0E, 0x00,
        0x09, 0xD8, 0x0E, 0x00, 0x0A, 0xD8, 0x0E, 0x00,
        0x0B, 0xD8, 0x0E, 0x00, 0x0C, 0xD8, 0x0E, 0x00,
        0x0D, 0xD8, 0x0E, 0x00, 0x0E, 0xD8, 0x0E, 0x00,
        0x0F, 0xD8, 0x0E, 0x00, 0x10, 0xD8, 0x0E, 0x00,
        0x37, 0xD8, 0x0E, 0x00, 0x38, 0xD8, 0x0E, 0x00,
        0x39, 0xD8, 0x0E, 0x00, 0x3A, 0xD8, 0x0E, 0x00,
        0x3B, 0xD8, 0x0E, 0x00, 0x3C, 0xD8, 0x0E, 0x00,
        0x3D, 0xD8, 0x0E, 0x00, 0x3E, 0xD8, 0x0E, 0x00,
        0x3F, 0xD8, 0x0E, 0x00, 0x40, 0xD8, 0x0E, 0x00
    };
    
    address = 0x0;
    memcpy((void *)(address + offset), buffer, 0x68);
  
    data = 0xED844;
   for (int i = 0; i < 0x2D; i++)
   {
       WRITEU32(0x68 + offset, data);
       offset += 4;
       data += 0x1;
   }
    
    offset = 0x330D5934;
    WRITEU32(0x11C + offset, 0xED874);
 
    static const u8    buffer2[] =
    {
        0x75, 0xD8, 0x0E, 0x00, 0x76, 0xD8, 0x0E, 0x00,
        0x77, 0xD8, 0x0E, 0x00, 0x87, 0xD8, 0x0E, 0x00,
        0x88, 0xD8, 0x0E, 0x00, 0x89, 0xD8, 0x0E, 0x00,
        0x8A, 0xD8, 0x0E, 0x00, 0x8B, 0xD8, 0x0E, 0x00,
        0x8C, 0xD8, 0x0E, 0x00, 0x8D, 0xD8, 0x0E, 0x00,
        0x8E, 0xD8, 0x0E, 0x00, 0x8F, 0xD8, 0x0E, 0x00,
        0x90, 0xD8, 0x0E, 0x00, 0x91, 0xD8, 0x0E, 0x00,
        0x92, 0xD8, 0x0E, 0x00, 0x93, 0xD8, 0x0E, 0x00,
        0x94, 0xD8, 0x0E, 0x00, 0xD5, 0xD8, 0x0E, 0x00,
        0xD6, 0xD8, 0x0E, 0x00, 0xD7, 0xD8, 0x0E, 0x00
    };

    address = 0x120;
    memcpy((void *)(address + offset), buffer2, 0x50);
    
    data = 0xED8D9;
   for (int i = 0; i < 0x6F; i++)
   {
       WRITEU32(0x170 + offset, data);
       offset += 4;
       data += 1;
    }
    
    offset = 0x330D5934;

    static const u8    buffer3[] =
    {
        0xE5, 0xD9, 0x0E, 0x00, 0xE6, 0xD9, 0x0E, 0x00,
        0xE7, 0xD9, 0x0E, 0x00, 0xE8, 0xD9, 0x0E, 0x00,
        0xE9, 0xD9, 0x0E, 0x00, 0xEA, 0xD9, 0x0E, 0x00,
        0xEB, 0xD9, 0x0E, 0x00, 0xEC, 0xD9, 0x0E, 0x00,
        0xED, 0xD9, 0x0E, 0x00, 0xEE, 0xD9, 0x0E, 0x00,
        0xEF, 0xD9, 0x0E, 0x00, 0xF0, 0xD9, 0x0E, 0x00,
        0xF1, 0xD9, 0x0E, 0x00, 0xF2, 0xD9, 0x0E, 0x00,
        0xF3, 0xD9, 0x0E, 0x00, 0xF4, 0xD9, 0x0E, 0x00,
        0xF9, 0xD9, 0x0E, 0x00, 0xFA, 0xD9, 0x0E, 0x00,
        0xFB, 0xD9, 0x0E, 0x00, 0xFC, 0xD9, 0x0E, 0x00,
        0xFD, 0xD9, 0x0E, 0x00, 0xFE, 0xD9, 0x0E, 0x00,
        0xFF, 0xD9, 0x0E, 0x00, 0x00, 0xDA, 0x0E, 0x00,
        0x01, 0xDA, 0x0E, 0x00, 0x02, 0xDA, 0x0E, 0x00,
        0x03, 0xDA, 0x0E, 0x00, 0x04, 0xDA, 0x0E, 0x00,
        0x05, 0xDA, 0x0E, 0x00, 0x06, 0xDA, 0x0E, 0x00,
        0x07, 0xDA, 0x0E, 0x00, 0x08, 0xDA, 0x0E, 0x00,
        0x09, 0xDA, 0x0E, 0x00, 0x0A, 0xDA, 0x0E, 0x00,
        0x0B, 0xDA, 0x0E, 0x00, 0x0C, 0xDA, 0x0E, 0x00,
        0x0D, 0xDA, 0x0E, 0x00, 0x0E, 0xDA, 0x0E, 0x00,
        0x0F, 0xDA, 0x0E, 0x00, 0x10, 0xDA, 0x0E, 0x00,
        0x11, 0xDA, 0x0E, 0x00, 0x12, 0xDA, 0x0E, 0x00,
        0x13, 0xDA, 0x0E, 0x00, 0x16, 0xDA, 0x0E, 0x00,
        0x17, 0xDA, 0x0E, 0x00, 0x19, 0xDA, 0x0E, 0x00,
        0x1A, 0xDA, 0x0E, 0x00, 0x1B, 0xDA, 0x0E, 0x00,
        0x1C, 0xDA, 0x0E, 0x00, 0x1D, 0xDA, 0x0E, 0x00,
        0x1E, 0xDA, 0x0E, 0x00, 0x1F, 0xDA, 0x0E, 0x00,
        0x20, 0xDA, 0x0E, 0x00, 0x21, 0xDA, 0x0E, 0x00,
        0x22, 0xDA, 0x0E, 0x00, 0x23, 0xDA, 0x0E, 0x00,
        0x24, 0xDA, 0x0E, 0x00, 0x25, 0xDA, 0x0E, 0x00,
        0x26, 0xDA, 0x0E, 0x00, 0x27, 0xDA, 0x0E, 0x00,
        0x28, 0xDA, 0x0E, 0x00, 0x29, 0xDA, 0x0E, 0x00,
        0x2A, 0xDA, 0x0E, 0x00, 0x2B, 0xDA, 0x0E, 0x00,
        0x2C, 0xDA, 0x0E, 0x00, 0x2D, 0xDA, 0x0E, 0x00,
        0x2E, 0xDA, 0x0E, 0x00, 0x2F, 0xDA, 0x0E, 0x00,
        0x30, 0xDA, 0x0E, 0x00, 0x31, 0xDA, 0x0E, 0x00,
        0x32, 0xDA, 0x0E, 0x00, 0x33, 0xDA, 0x0E, 0x00,
        0x34, 0xDA, 0x0E, 0x00, 0x3B, 0xDA, 0x0E, 0x00,
        0x3C, 0xDA, 0x0E, 0x00, 0x3D, 0xDA, 0x0E, 0x00,
        0x3F, 0xDA, 0x0E, 0x00, 0x40, 0xDA, 0x0E, 0x00,
        0x41, 0xDA, 0x0E, 0x00, 0x44, 0xDA, 0x0E, 0x00,
        0x45, 0xDA, 0x0E, 0x00, 0x46, 0xDA, 0x0E, 0x00,
        0x47, 0xDA, 0x0E, 0x00, 0x48, 0xDA, 0x0E, 0x00,
        0x49, 0xDA, 0x0E, 0x00, 0x4A, 0xDA, 0x0E, 0x00,
        0x4B, 0xDA, 0x0E, 0x00, 0x4C, 0xDA, 0x0E, 0x00,
        0x4D, 0xDA, 0x0E, 0x00, 0x4E, 0xDA, 0x0E, 0x00,
        0x50, 0xDA, 0x0E, 0x00, 0x51, 0xDA, 0x0E, 0x00,
        0x52, 0xDA, 0x0E, 0x00, 0x53, 0xDA, 0x0E, 0x00,
        0x54, 0xDA, 0x0E, 0x00, 0x55, 0xDA, 0x0E, 0x00,
        0x56, 0xDA, 0x0E, 0x00, 0x57, 0xDA, 0x0E, 0x00,
        0x58, 0xDA, 0x0E, 0x00, 0x59, 0xDA, 0x0E, 0x00,
        0x5A, 0xDA, 0x0E, 0x00, 0x5B, 0xDA, 0x0E, 0x00,
        0x5C, 0xDA, 0x0E, 0x00, 0x5D, 0xDA, 0x0E, 0x00,
        0x5E, 0xDA, 0x0E, 0x00, 0x5F, 0xDA, 0x0E, 0x00,
        0x60, 0xDA, 0x0E, 0x00, 0x61, 0xDA, 0x0E, 0x00,
        0x62, 0xDA, 0x0E, 0x00, 0x63, 0xDA, 0x0E, 0x00,
        0x64, 0xDA, 0x0E, 0x00, 0x65, 0xDA, 0x0E, 0x00,
        0x66, 0xDA, 0x0E, 0x00, 0x67, 0xDA, 0x0E, 0x00,
        0x7F, 0xDA, 0x0E, 0x00, 0x80, 0xDA, 0x0E, 0x00,
        0x84, 0xDA, 0x0E, 0x00, 0x86, 0xDA, 0x0E, 0x00,
        0x87, 0xDA, 0x0E, 0x00, 0x88, 0xDA, 0x0E, 0x00,
        0x89, 0xDA, 0x0E, 0x00, 0x8A, 0xDA, 0x0E, 0x00
    };
    
    address = 0x32C;
    memcpy((void *)(address + offset), buffer3, 0x1E8);
 
    data = 0xEDA8C;
    for (int i = 0; i < 0x22; i++)
    {
        WRITEU32(0x514 + offset, data);
        offset += 4;
        data += 1;
    }

    offset = 0x330D5934;
    WRITEU32(0x59C + offset, 0xEDABB);

    static const u8    buffer4[] =
    {
        0xC0, 0xDA, 0x0E, 0x00, 0xC6, 0xDA, 0x0E, 0x00,
        0xC7, 0xDA, 0x0E, 0x00, 0xCB, 0xDA, 0x0E, 0x00,
        0xF0, 0xDA, 0x0E, 0x00, 0xF1, 0xDA, 0x0E, 0x00,
        0xF2, 0xDA, 0x0E, 0x00, 0xF3, 0xDA, 0x0E, 0x00,
        0xF4, 0xDA, 0x0E, 0x00, 0xF5, 0xDA, 0x0E, 0x00,
        0xF6, 0xDA, 0x0E, 0x00, 0xF7, 0xDA, 0x0E, 0x00,
        0xF8, 0xDA, 0x0E, 0x00, 0xF9, 0xDA, 0x0E, 0x00,
        0xFA, 0xDA, 0x0E, 0x00, 0xFB, 0xDA, 0x0E, 0x00,
        0xFC, 0xDA, 0x0E, 0x00, 0xFF, 0xDA, 0x0E, 0x00,
        0x00, 0xDB, 0x0E, 0x00, 0x01, 0xDB, 0x0E, 0x00,
        0x02, 0xDB, 0x0E, 0x00, 0x1B, 0xDB, 0x0E, 0x00,
        0x1C, 0xDB, 0x0E, 0x00, 0x4C, 0xDB, 0x0E, 0x00,
        0x4E, 0xDB, 0x0E, 0x00, 0x51, 0xDB, 0x0E, 0x00,
        0x53, 0xDB, 0x0E, 0x00, 0x55, 0xDB, 0x0E, 0x00,
        0x56, 0xDB, 0x0E, 0x00, 0x57, 0xDB, 0x0E, 0x00,
        0x58, 0xDB, 0x0E, 0x00, 0x6F, 0xDB, 0x0E, 0x00,
        0x70, 0xDB, 0x0E, 0x00, 0x71, 0xDB, 0x0E, 0x00,
        0x72, 0xDB, 0x0E, 0x00, 0x73, 0xDB, 0x0E, 0x00,
        0x74, 0xDB, 0x0E, 0x00, 0x88, 0xDB, 0x0E, 0x00,
        0x89, 0xDB, 0x0E, 0x00, 0x8A, 0xDB, 0x0E, 0x00,
        0x8B, 0xDB, 0x0E, 0x00, 0x8C, 0xDB, 0x0E, 0x00,
        0x8D, 0xDB, 0x0E, 0x00, 0x8E, 0xDB, 0x0E, 0x00,
        0x8F, 0xDB, 0x0E, 0x00, 0x90, 0xDB, 0x0E, 0x00,
        0x91, 0xDB, 0x0E, 0x00, 0x92, 0xDB, 0x0E, 0x00,
        0x93, 0xDB, 0x0E, 0x00, 0x94, 0xDB, 0x0E, 0x00,
        0x95, 0xDB, 0x0E, 0x00, 0x96, 0xDB, 0x0E, 0x00,
        0x97, 0xDB, 0x0E, 0x00, 0x98, 0xDB, 0x0E, 0x00
    };
    
    address = 0x5A0;
    memcpy((void *)(address + offset), buffer4, 0xD8);

    if (any_is_pressed(0x0000CFFE)) // Any button but A
        disableCheat(i_allItems);
}


// Gives all medicine items and sets them to quantity 950 (credit to Nanquitas for method)
void    allMedicine(void)
{
    u32    data;
    u32    offset;
    u32    address;

    data = 0;
    offset = 0;
    static const u8    buffer[] =
    { 
        0x41, 0xD8, 0x0E, 0x00, 0x42, 0xD8, 0x0E, 0x00, 
        0x43, 0xD8, 0x0E, 0x00, 0x86, 0xD8, 0x0E, 0x00,
        0xF8, 0xD9, 0x0E, 0x00, 0x35, 0xDA, 0x0E, 0x00, 
        0x36, 0xDA, 0x0E, 0x00, 0x37, 0xDA, 0x0E, 0x00, 
        0x38, 0xDA, 0x0E, 0x00, 0x39, 0xDA, 0x0E, 0x00, 
        0x3A, 0xDA, 0x0E, 0x00, 0x4F, 0xDA, 0x0E, 0x00, 
        0x85, 0xDA, 0x0E, 0x00, 0xC4, 0xDA, 0x0E, 0x00, 
        0xC5, 0xDA, 0x0E, 0x00, 0x54, 0xDB, 0x0E, 0x00
    };
        
	offset = 0x330D647C;
    data = 0x000ED811;
    for (int i = 0; i < 0x26; i++)
    {
        WRITEU32(0x0 + offset, data);
        offset += 4;
        data += 0x1;
    }
    offset = 0x330D647C;
    address = 0x00000098;
    memcpy((void *)(address + offset), buffer, 0x40);
	
    if (any_is_pressed(0x0000CFFE)) // Any button but A
		disableCheat(i_allMedicine);
}

// Gives all TMs
void    allTMs(void)
{
    static const u8 buffer[] =
    {
        0x6A, 0x06, 0x00, 0x00, 0x6B, 0x06, 0x00, 0x00,
        0x6C, 0x06, 0x00, 0x00, 0xB2, 0x06, 0x00, 0x00,
        0xB3, 0x06, 0x00, 0x00, 0xB4, 0x06, 0x00, 0x00,
        0xB5, 0x06, 0x00, 0x00, 0xB6, 0x06, 0x00, 0x00,
        0xA4, 0x05, 0x00, 0x00, 0xA5, 0x05, 0x00, 0x00,
        0xA6, 0x05, 0x00, 0x00, 0xA7, 0x05, 0x00, 0x00,
        0xA8, 0x05, 0x00, 0x00, 0xA9, 0x05, 0x00, 0x00,
    };
    
    u32 data;
    u32 offset;
    u32 address;
    
    offset = 0x330D62CC;
    data = 0x548;
    for (int i = 0; i < 0x5C; i++)
    {
        WRITEU32(offset, data);
        offset += 4;
        data += 1;
    }
    address = 0x330D62CC;
    offset = 0x170;
    memcpy((void *)(address + offset), buffer, 0x38);
    WRITEU32(address + 0x1A8, 0x6E1);
    
    if (any_is_pressed(0x0000CFFE)) // Any button but A
        disableCheat(i_allTMs);
}

// Gives all berries and sets them to quantity 950 (credit to Nanquitas for method)
void    allBerries(void)
{
    u32    data;
    u32    offset;

    data = 0;
    offset = 0;
  
	offset = 0x330D657C;
    data = 0x000ED895;
    for (int i = 0; i < 0x40; i++)
    {
        WRITEU32(0x0 + offset, data);
        offset += 4;
        data += 0x00000001;
    }
    offset = 0x330D657C;
	
	WRITEU32(0x100 + offset, 0xEDAAE);
	WRITEU32(0x104 + offset, 0xEDAAF);
	WRITEU32(0x108 + offset, 0xEDAB0);
	
    if (any_is_pressed(0x0000CFFE)) // Any button but A
		disableCheat(i_allBerries);
}

// Function to add clothes to inventory
u32		clothesFunction(u32 offset, u8 data, u32 loop_num)
{
	for (int i = 0; i < loop_num; i++)
	{
		WRITEU8(0x0 + offset, data);
		offset += 1;
	}
	return(offset);
}

// All Clothes
void	allClothes(void)
{
	u32 offset = 0x33116620;
	
    if (READU8(0x330D67D5) == 0x0)
    {
        offset = clothesFunction(offset, 0x1, 0x3A);
        offset = clothesFunction(offset, 0x0, 0x87);
        offset = clothesFunction(offset, 0x1, 0x14);
        offset = clothesFunction(offset, 0x0, 0x20);
        offset = clothesFunction(offset, 0x1, 0x100);
        offset = clothesFunction(offset, 0x0, 0x8F);
        offset = clothesFunction(offset, 0x1, 0x69);
        offset = clothesFunction(offset, 0x0, 0xC7);
        offset = clothesFunction(offset, 0x1, 0x25);
        offset = clothesFunction(offset, 0x0, 0x79);
        offset = clothesFunction(offset, 0x1, 0x5F);
        offset = clothesFunction(offset, 0x0, 0x66);
        offset = clothesFunction(offset, 0x1, 0x6D);
        clothesFunction(offset, 0x0, 0x44);
    } else {
        offset = clothesFunction(offset, 0x1, 0x5E);
        offset = clothesFunction(offset, 0x0, 0x20);
        offset = clothesFunction(offset, 0x1, 0x23);
        offset = clothesFunction(offset, 0x0, 0x20);
        offset = clothesFunction(offset, 0x1, 0x14);
        offset = clothesFunction(offset, 0x0, 0x20);
        offset = clothesFunction(offset, 0x1, 0x18A);
        offset = clothesFunction(offset, 0x0, 0x5);
        offset = clothesFunction(offset, 0x1, 0xF9);
        offset = clothesFunction(offset, 0x0, 0x37);
        offset = clothesFunction(offset, 0x1, 0x7E);
        offset = clothesFunction(offset, 0x0, 0x20);
        offset = clothesFunction(offset, 0x1, 0xA7);
        offset = clothesFunction(offset, 0x0, 0x1E);
        offset = clothesFunction(offset, 0x1, 0x91);
        clothesFunction(offset, 0x0, 0x20);
    }
	
    offset = 0x33116620;
    
	WRITEU32(0xF8 + offset, 0x3010101);
	WRITEU32(0x2C8 + offset, 0x3010101);
	WRITEU32(0x3B4 + offset, 0x1010103);
	WRITEU32(0x450 + offset, 0x1030000);
	WRITEU32(0x514 + offset, 0x3000000);
	
	if (any_is_pressed(0x0000CFFE))
		disableCheat(i_allClothes);
}

/********************************
*				*
*		Misc		*
*				*
********************************/

// Sets text speed to instant
void	instantText(void)
{
	WRITEU32(0x3BF26C, 0xE3A05003);
	WRITEU32(0x3BE9C8, 0xE3A04003);
}

// Access PC from anywhere by holding START while opening options menu
void    pcAnywhere(void)
{
    u32    offset;
    u32    address;
    
    static const u8    buffer[] =
    {
        0x0E, 0x40, 0x2D, 0xE9, 0x74, 0x10, 0x9F, 0xE5,
        0x00, 0x20, 0x91, 0xE5, 0x00, 0x30, 0xA0, 0xE3,
        0x00, 0x30, 0x81, 0xE5, 0x13, 0x3C, 0xA0, 0xE3,
        0x93, 0x30, 0x83, 0xE2, 0x03, 0x00, 0x52, 0xE1,
        0x00, 0x40, 0xA0, 0x03, 0x00, 0x40, 0xA0, 0x11,
        0x0E, 0x80, 0xBD, 0xE8, 0x0E, 0x00, 0x50, 0xE3,
        0x09, 0x7D, 0xF7, 0xEA, 0x0F, 0x40, 0x2D, 0xE9,
        0x10, 0xD0, 0x4D, 0xE2, 0x00, 0x00, 0xA0, 0xE3,
        0x00, 0x00, 0x8D, 0xE5, 0x04, 0x00, 0x8D, 0xE5,
        0x08, 0x00, 0x8D, 0xE5, 0x0C, 0x00, 0x8D, 0xE5,
        0x00, 0x30, 0xA0, 0xE1, 0x00, 0x20, 0xA0, 0xE1,
        0x13, 0x1C, 0xA0, 0xE3, 0x93, 0x10, 0x81, 0xE2,
        0x18, 0x00, 0x9F, 0xE5, 0x00, 0x10, 0x80, 0xE5,
        0x14, 0x00, 0x9F, 0xE5, 0x00, 0x00, 0x90, 0xE5,
        0xCE, 0xD9, 0xF7, 0xEB, 0x02, 0x00, 0xA0, 0xE3,
        0x10, 0xD0, 0x8D, 0xE2, 0x0F, 0x80, 0xBD, 0xE8,
        0x00, 0x60, 0x63, 0x00, 0x6C, 0x20, 0x67, 0x00
    };
    
    offset = 0x0;
    address = 0x595978;
    memcpy((void *)(address + offset), buffer, 0x88);
    
    WRITEU32(0x37D560, 0xEB086104);
    WRITEU32(0x373C48, 0xEB088755);
    
    if (is_pressed(BUTTON_ST))
        WRITEU32(0x5959A8, 0x1AF77D09);
}

// Re-battle trainer that have already been fought. Active by holding L and talking to them
void	rematchTrainers(void)
{
	if (!is_pressed(BUTTON_L))
	{
		WRITEU32(0x49D200, 0xE5911004);
		WRITEU32(0x49D204, 0xE5900044);
	} else {
		WRITEU32(0x49D200, 0xE3A00000);
		WRITEU32(0x49D204, 0xE12FFF1E);
	}
}

// Toggles model outlines for player and Pokemon off and on
void	toggleOutlines(void)
{
	if (READU32(0x41B748) == 0xE5802004)
		WRITEU32(0x41B748, 0xE320F000);
	else
		WRITEU32(0x41B748, 0xE5802004);
	updateOutlines();
	disableCheat(i_toggleOutlines);
}

// Updates code name on menu depending on current enable / disable status
void	updateOutlines(void)
{
	if (READU32(0x41B748) == 0xE5802004)
		xsprintf(statusOutlines, "Disable Outlines *");
	else
		xsprintf(statusOutlines, "Enable Outlines  *");
}
// Changes current gender and skin tone. Sets clothes and hair to default with gender change.
void    switchLooks(void)
{
    u32 offset = 0x330D67D4;
    
    if (currentGender == 0x00 && currentGender != READU8(0x330D67D5))
    {
        WRITEU8(0x01 + offset, currentGender);
        WRITEU32(0x50 + offset, 0x800000 + (matchingHair * 0x100) + currentSkintone);
        WRITEU32(0x54 + offset, 0x40000);
        WRITEU32(0x58 + offset, 0x10040C);
        WRITEU32(0x5C + offset, 0x1C01);
    } else if (currentGender == 0x01 && currentGender != READU8(0x330D67D5))
    {
        WRITEU8(0x01 + offset, currentGender);
        WRITEU32(0x50 + offset, 0x800000 + (matchingHair * 0x100) + currentSkintone);
        WRITEU32(0x54 + offset, 0x40000);
        WRITEU32(0x58 + offset, 0x100405);
        WRITEU32(0x5C + offset, 0x3001);
    } else {
        WRITEU8(0x50 + offset, currentSkintone);
    }
    
    WRITEU32(0x64 + offset, 0x00);
        
    if (any_is_pressed(0x0000CFFE))
        disableCheat(i_switchLooks);
}

// Updates menu with current gender
void    updateGender(void)
{
    if (currentGender == 0x00)
        xsprintf(storedGender, "Current Gender: Male   *");
    if (currentGender == 0x01)
        xsprintf(storedGender, "Current Gender: Female *");
}

// Updates menu with current skintone and changes hair color to match
void    updateSkintone(void)
{
    if (currentSkintone == 0x00 || currentSkintone == 0x04)
    {
        matchingHair = 0x83;
        xsprintf(storedSkintone, "Current Skintone: A *");
    }
    if (currentSkintone == 0x08 || currentSkintone == 0x0C)
    {
        matchingHair = 0x04;
        xsprintf(storedSkintone, "Current Skintone: B *");
    }
    if (currentSkintone == 0x10 || currentSkintone == 0x14)
    {
        matchingHair = 0x48;
        xsprintf(storedSkintone, "Current Skintone: C *");
    }
    if (currentSkintone == 0x18 || currentSkintone == 0x1C)
    {
        matchingHair = 0x08;
        xsprintf(storedSkintone, "Current Skintone: D *");
    }
}

// Cycles through gender choices
void    setGender(void)
{
    if (currentGender == 0x00)
    {
        currentGender++;
        currentSkintone += 0x4;
    } else {
        currentGender--;
        currentSkintone -= 0x4;
    }
    
    updateGender();
    disableCheat(i_setGender);
}

// Cycles through skintone choices
void    setSkintone(void)
{
    if (currentSkintone == 0x18)
        currentSkintone = 0x0;
    else if (currentSkintone == 0x1C)
        currentSkintone = 0x4;
    else
        currentSkintone += 0x8;
    
    updateSkintone();
    disableCheat(i_setSkintone);
}
