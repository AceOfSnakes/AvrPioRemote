#include "zonenumberbase.h"

ZoneNumberBase::ZoneNumberBase() :
    m_Zone(ZoneMain)
{

}

ZoneNumberBase::~ZoneNumberBase()
{

}

ZoneNumberBase::Zone ZoneNumberBase::GetZone()
{
    return m_Zone;
}

