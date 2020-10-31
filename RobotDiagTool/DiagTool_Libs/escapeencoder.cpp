#include "EscapeEncoder.h"

/* Minden frame-et '\n' (0xA) kell hogy zárjon. Választunk vmi ritkán használt
 * escape karaktert (pl. 0x42), ami escape-eli magát (0x4242) és a 0xA értékű
 * bájtokat (0x420A). Ez azért jó, mert így egy frame egy sor, amit a Qt és a
 * legtöbb PC-s alkalmazás így tud a legegyszerűbben fogadni. A sorok elejére
 * még lejetne rakni egy vezérlő karaktert, ami megmondja, hogy bájtonként
 * vagy ASCII-ként kell-e értelmezni az üzenetet és  így a válogatás is megvan.
 *
 * (Lehet, hogy a \r-t is escape-elni kell, ez majd a Qt-tól fog függeni) */

#define ESCAPE_CHAR           (0x55)
#define LINEFEED_SUBSTITUTE   (0x00)

void EscapeEncoder::Encode(const uint8_t* bin, size_t binSize, uint8_t* enc, size_t& encSize)
{
    size_t iBin;
    size_t iEnc = 0;

    for (iBin = 0; iBin < binSize; iBin++)
    {
        if (bin[iBin] == '\n')
        {
            enc[iEnc] = ESCAPE_CHAR;
            iEnc++;
            enc[iEnc] = LINEFEED_SUBSTITUTE;
        }
        else if (bin[iBin] == ESCAPE_CHAR)
        {
            enc[iEnc] = ESCAPE_CHAR;
            iEnc++;
            enc[iEnc] = ESCAPE_CHAR;
        }
        else
        {
            enc[iEnc] = bin[iBin];

        }

        iEnc++;
    }

    encSize = iEnc;
}

void EscapeEncoder::Decode(const uint8_t* enc, size_t encSize, uint8_t* bin, size_t& binSize)
{
    size_t iBin = 0;
    size_t iEnc;

    for (iEnc = 0; iEnc < encSize; iEnc++)
    {
        if (enc[iEnc] == ESCAPE_CHAR)
        {
            iEnc++;

            if (enc[iEnc] == LINEFEED_SUBSTITUTE)
            {
                bin[iBin] = '\n';
            }
            else if (enc[iEnc] == ESCAPE_CHAR)
            {
                bin[iBin] = ESCAPE_CHAR;
            }
            else
            {
                // Error
            }

            iBin++;
        }
        else
        {
            bin[iBin] = enc[iEnc];
            iBin++;
        }
    }

    binSize = iBin;
}
