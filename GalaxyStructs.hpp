#ifndef GALAXYSTRUCTS_HPP
#define GALAXYSTRUCTS_HPP

#include <vector>
#include <map>
#include <string>
#include <experimental/optional>

using namespace std;
using namespace std::experimental;

struct System {
    double x;
    double y;
    double z;
    unsigned long int id64;
    bool isNeutron;
    bool isScoopable;
    bool hasIcyRing;
    bool isPristine;
};

struct SystemExtra {
    string name;
};

struct Ring {
    string type;
};

struct Body {
    unsigned long int id64;
    bool isMainStar;
    unsigned long int distanceToArrival;
    string reserveLevel;
    string type;
    string subtype;
};

enum class ParseSection {
    none,
    system,
    controlling_faction,
    coords,
    bodies,
    body,
    solid_compositions,
    atmosphere_compositions,
    materials,
    belts,
    belt,
    rings,
    ring,
    stations,
    discovery,
    parents,
    factions,
    faction_active_states,
    faction_pending_states,
    faction_recovering_states,
    import_commodities,
    export_commodities,
    prohibited_commodities,
    market,
    ships,
    modules,
    services,
	same
};

enum class ParseObject {
    none, ring, body, system, system_extra
};

enum class ParseTypes {
    unknown, integer, real, scalar, boolean,
    optional_integer, optional_real, optional_scalar, optional_boolean,
    vector_scalar
};

enum class BodyTypes {
    star,
    planet
};

enum class BodySubtypes {
    unknown,
    neutron_star,
    a_star,
    b_star,
    f_star,
    g_star,
    k_star,
    m_star,
    o_star
};

struct ParsedField {
    bool hasKey;
    ParseObject object;
    bool isFinishedObject;
    string identifier;
};

struct KeyLookup {
    const char *name;
    ParseSection section;
    ParseObject object;
    bool isFinishedObject;
    long int offset;
	ParseTypes next_type;
};

#endif
