#ifndef ZONENUMBERBASE_H
#define ZONENUMBERBASE_H


class ZoneNumberBase
{
public:
    ZoneNumberBase();
    ~ZoneNumberBase();
    enum Zone {
        ZoneMain,
        Zone2,
        Zone3,
        Zone4
    };
    Zone GetZone();

protected:
    Zone    m_Zone;
};

#endif // ZONENUMBERBASE_H
