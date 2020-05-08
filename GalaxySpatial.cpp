#define RAPIDJSON_SSE42

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/istreamwrapper.h"
#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <experimental/optional>

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

#include <boost/algorithm/string/join.hpp>

#include "GalaxyHash.hpp"
#include "BodyTypesHash.hpp"
#include "BodySubtypesHash.hpp"
#include "GalaxyStructs.hpp"

#include <spatialindex/capi/sidx_api.h>
#include <spatialindex/capi/sidx_impl.h>
#include <spatialindex/capi/sidx_config.h>

#define REPORT_SIZE 100000
#define CLOSE_STAR 100
#define LIMIT 0

using namespace std;
using namespace std::experimental;
using namespace std::chrono;
using namespace rapidjson;
using namespace SpatialIndex;

inline string getHeirarchy(vector<ParsedField> parsedFields) {
    string current = "";
    for (auto& parsedField : parsedFields) {
        if (parsedField.identifier.length()) {
            current += parsedField.identifier;
        }
    }
    return current;
}


void debugHeirarchy(vector<ParsedField> parsedFields) {
    for (auto& parsedField : parsedFields) {
        if (parsedField.identifier.length()) {
            cerr << parsedField.identifier;
            /*
            if (parsedField.hasKey) {
                cerr << "(true)";
            } else {
                cerr << "(false)";
            }
            */
        }
    }
    cerr << endl;
}

class Handler {
    protected:
        unsigned long int level;
        ParseSection currentSection;
        ParseObject currentObject;
        ParseTypes nextType;
        void *nextField;
        bool hasKey;
        bool inArray;
        vector<ParsedField> heirarchy;
    public: 
        bool isFinishedSystem;
        bool isFinishedBody;
        System system;
        SystemExtra system_extra;
        Body body;
        Ring ring;
        
        Handler() {
            this->level = 0;
            this->nextField = NULL;
            this->hasKey = false;
            this->isFinishedSystem = false;
            this->currentSection = ParseSection::none;
            this->isFinishedBody = false;
            this->inArray = false;
        }
        bool Null() {
            if (!this->inArray) {
                this->heirarchy.pop_back();
            }
            this->hasKey = false;
            return true;
        }
        bool Bool(bool b) {
            if (this->nextField != NULL) {
                if (this->nextType == ParseTypes::boolean) {
                    *(bool*)this->nextField = b;
                } else {
                    cerr << "Bool value was not recognised " << b << " Type: " << (unsigned int) this->nextType << " should have been " << (unsigned int) ParseTypes::boolean << endl;
                    throw "Bool value was not recognised";
                }
            }
            if (!this->inArray) {
                this->heirarchy.pop_back();
            }
            this->hasKey = false;
            return true;
        }
        bool Int(int value) {
            if (this->nextField != NULL) {
                if (this->nextType == ParseTypes::integer) {
                    *(unsigned long int*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::optional_integer) {
                    *(optional<unsigned long int>*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::real) {
                    *(double*)this->nextField = (double) value;
                } else if (this->nextType == ParseTypes::optional_real) {
                    *(optional<double>*)this->nextField = (double) value;
                } else {
                    cerr << (string) __func__ << "value was not recognised " << value << " Type: " << (unsigned int) this->nextType << endl;
                    throw (string) __func__ + " value was not recognised";
                }
            }
            if (!this->inArray) {
                this->heirarchy.pop_back();
            }
            this->hasKey = false;
            return true;
        }
        bool Uint(unsigned value) {
            if (this->nextField != NULL) {
                if (this->nextType == ParseTypes::integer) {
                    *(unsigned long int*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::optional_integer) {
                    *(optional<unsigned long int>*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::real) {
                    *(double*)this->nextField = (double) value;
                } else if (this->nextType == ParseTypes::optional_real) {
                    *(optional<double>*)this->nextField = (double) value;
                } else {
                    cerr << (string) __func__ << "value was not recognised " << value << " Type: " << (unsigned int) this->nextType << endl;
                    throw (string) __func__ + " value was not recognised";
                }
            }
            if (!this->inArray) {
                this->heirarchy.pop_back();
            }
            this->hasKey = false;
            return true;
        }
        bool Int64(int64_t value) {
            if (this->nextField != NULL) {
                if (this->nextType == ParseTypes::integer) {
                    *(unsigned long int*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::optional_integer) {
                    *(optional<unsigned long int>*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::real) {
                    *(double*)this->nextField = (double) value;
                } else if (this->nextType == ParseTypes::optional_real) {
                    *(optional<double>*)this->nextField = (double) value;
                } else {
                    cerr << (string) __func__ << "value was not recognised " << value << " Type: " << (unsigned int) this->nextType << endl;
                    throw (string) __func__ + " value was not recognised";
                }
            }
            if (!this->inArray) {
                this->heirarchy.pop_back();
            }
            this->hasKey = false;
            return true;
        }
        bool Uint64(uint64_t value) {
            if (this->nextField != NULL) {
                if (this->nextType == ParseTypes::integer) {
                    *(unsigned long int*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::optional_integer) {
                    *(optional<unsigned long int>*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::real) {
                    *(double*)this->nextField = (double) value;
                } else if (this->nextType == ParseTypes::optional_real) {
                    *(optional<double>*)this->nextField = (double) value;
                } else {
                    cerr << (string) __func__ << "value was not recognised " << value << " Type: " << (unsigned int) this->nextType << endl;
                    throw (string) __func__ + " value was not recognised";
                }
            }
            if (!this->inArray) {
                this->heirarchy.pop_back();
            }
            this->hasKey = false;
            return true;
        }
        bool Double(double value) {
            if (this->nextField != NULL) {
                if (this->nextType == ParseTypes::integer) {
                    *(unsigned long int*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::optional_integer) {
                    *(optional<unsigned long int>*)this->nextField = (unsigned long int) value;
                } else if (this->nextType == ParseTypes::real) {
                    *(double*)this->nextField = (double) value;
                } else if (this->nextType == ParseTypes::optional_real) {
                    *(optional<double>*)this->nextField = (double) value;
                } else {
                    cerr << (string) __func__ << "value was not recognised " << value << " Type: " << (unsigned int) this->nextType << endl;
                    throw (string) __func__ + " value was not recognised";
                }
            }
            if (!this->inArray) {
                this->heirarchy.pop_back();
            }
            this->hasKey = false;
            return true;
        }
        bool RawNumber(const char* str, SizeType length, bool copy) {
            (void) str;
            (void) copy;
            (void) length;
            if (!this->inArray) {
                this->heirarchy.pop_back();
            }
            this->hasKey = false;
            return true;
        }
        bool String(const char* str, SizeType length, bool copy) {
            (void) copy;
            (void) length;
            if (this->nextField != NULL) {
                if (this->nextType == ParseTypes::scalar) {
                    *(string*)this->nextField = str;
                } else {
                    cerr << "String value was not recognised " << str << " Type: " << (unsigned int) this->nextType << endl;
                    throw "String value was not recognised";
                }
            }
            if (!this->inArray) {
                this->heirarchy.pop_back();
            }
            this->hasKey = false;
            return true;
        }
        bool Key(const char* str, SizeType length, bool copy) {
            (void)copy;

            this->hasKey = true;
            this->inArray = false;

            string key(str,length);
            this->heirarchy.push_back({true,ParseObject::none,false,key});
            // debugHeirarchy(heirarchy);
            string currentHeirarchy = getHeirarchy(this->heirarchy);

            const KeyLookup* lookup = GalaxyKey::Lookup(currentHeirarchy.c_str(),currentHeirarchy.length());

            if (lookup != NULL) {
                this->heirarchy.back().object = lookup->object;
                this->heirarchy.back().isFinishedObject = lookup->isFinishedObject;
                // cerr << "Found match for " << currentHeirarchy << endl;
                this->nextType = lookup->next_type;
                if (lookup->offset >= 0) {
                    switch (lookup->object) {
                        case ParseObject::system:
                            this->nextField = ((char *)(&this->system)) + lookup->offset;
                            break;
                        case ParseObject::system_extra:
                            this->nextField = ((char *)(&this->system_extra)) + lookup->offset;
                            break;
                        case ParseObject::body:
                            this->nextField = ((char *)(&this->body)) + lookup->offset;
                            break;
                        case ParseObject::ring:
                            this->nextField = ((char *)(&this->ring)) + lookup->offset;
                            break;
                        default:
                            break;
                    }
                }
            } else {
                // cerr << "Found no match for " << currentHeirarchy << endl;
                this->nextField = NULL;
            }

            return true;
        }
        bool StartObject() {
            this->heirarchy.push_back({this->hasKey,this->heirarchy.back().object,this->heirarchy.back().isFinishedObject,"->"});
            // debugHeirarchy(heirarchy);
            this->level++;
            if (this->heirarchy.back().isFinishedObject) {
                switch (this->heirarchy.back().object) {
                    case ParseObject::system:
                        this->system = {};
                        this->system_extra = {};
                        this->isFinishedSystem = false;
                        break;
                    case ParseObject::body:
                        this->body = {};
                        break;
                    case ParseObject::ring:
                        this->ring = {};
                        break;
                    default:
                        break;
                }
            }
            this->hasKey = false;
            return true;
        }
        bool EndObject(SizeType memberCount) {
            (void) memberCount;
            this->level--;
            ParsedField parsedField = this->heirarchy.back();
            this->heirarchy.pop_back();
            if (parsedField.hasKey) {
                this->heirarchy.pop_back();
            }
            // debugHeirarchy(heirarchy);
            if (parsedField.isFinishedObject) {
                switch (parsedField.object) {
                    case ParseObject::system:
                        this->isFinishedSystem = true;
                        break;
                    case ParseObject::body: {
                        // cerr << this->system.id64 << " has body subtype " << this->body.subtype << endl;

                        const BodySubtypeHash* lookup = BodySubtypeKey::Lookup(this->body.subtype.c_str(),this->body.subtype.length());
                        if (lookup != NULL) {
                            if (this->body.isMainStar) {
                                if (lookup->subtype == BodySubtypes::neutron_star) {
                                    this->system.isNeutron = true;
                                } else if (lookup->isScoopable) {
                                    this->system.isScoopable = true;
                                }
                            } else {
                                if (this->body.distanceToArrival < CLOSE_STAR) {
                                    if (lookup->isScoopable) {
                                        this->system.isScoopable = true;
                                    }
                                    if (lookup->subtype == BodySubtypes::neutron_star) {
                                        this->system.isNeutron = true;
                                    }
                                }
                            }
                        }
                        if (this->body.reserveLevel == "Pristine") {
                            this->system.isPristine = true;
                        }
                        break;
                    }
                    case ParseObject::ring:
                        // cerr << this->system.id64 << " has ring type " << this->ring.type << endl;
                        if (this->ring.type == "Icy") {
                            this->system.hasIcyRing = true;
                        }
                        break;
                    default:
                        break;
                }
            }

            this->hasKey = false;
            return true;
        }
        bool StartArray() {
            this->level++;
            if (this->heirarchy.size() > 0) {
                this->heirarchy.push_back({this->hasKey,this->heirarchy.back().object,this->heirarchy.back().isFinishedObject,"[]"});
            } else {
                this->heirarchy.push_back({this->hasKey,ParseObject::system,true,"[]"});
            }
            // debugHeirarchy(heirarchy);
            this->hasKey = false;
            this->inArray = true;
            return true;
        }
        bool EndArray(SizeType elementCount) {
            (void) elementCount;
            ParsedField parsedField = this->heirarchy.back();
            this->heirarchy.pop_back();
            if (parsedField.hasKey) {
                this->heirarchy.pop_back();
            }
            // debugHeirarchy(heirarchy);
            this->level--;

            this->hasKey = false;
            return true;
        }
};

class MyDataStream : public IDataStream {
protected:
    string filename;
    boost::iostreams::filtering_istream inputStream;
    Handler handler;
    Reader reader;
    IStreamWrapper* stream;
    high_resolution_clock::time_point t1,t2,t3;
    unsigned long int count;
public:
    MyDataStream(string filename) : m_pNext(NULL) {
        this->filename = filename;

        inputStream.push(boost::iostreams::bzip2_decompressor());
        inputStream.push(boost::iostreams::file_source(filename));

        this->stream = new IStreamWrapper(inputStream);

        this->reader.IterativeParseInit();
        this->count = 0;

        this->t1 = high_resolution_clock::now();
        this->t3 = this->t1;

        this->readNextEntry();
    }
    ~MyDataStream() override
    {
        if (this->m_pNext != NULL) delete this->m_pNext;
    }

    IData* getNext() override
    {
        if (this->m_pNext == NULL) return NULL;

        RTree::Data* ret = this->m_pNext;
        this->m_pNext = NULL;
        this->readNextEntry();
        return ret;
    }

    bool hasNext() override
    {
        return (this->m_pNext != NULL);
    }

    uint32_t size() override
    {
        throw Tools::NotSupportedException("Operation not supported.");
    }

    void rewind() override
    {
        throw Tools::NotSupportedException("Operation not supported.");
    }

    void readNextEntry()
    {
        if (LIMIT > 0 && this->count > LIMIT) {
            this->m_pNext = NULL;
            return;
        }
        while (!this->reader.IterativeParseComplete() && !this->reader.HasParseError()) {
            this->reader.template IterativeParseNext<kParseDefaultFlags>(*stream, handler);
            if (this->handler.isFinishedSystem) {
                this->count++;
                if (this->count % REPORT_SIZE == 0) {
                    this->t2 = high_resolution_clock::now();
                    duration<double> total_time_span = duration_cast<duration<double>>(this->t2 - this->t1);
                    duration<double> last_time_span = duration_cast<duration<double>>(this->t2 - this->t3);
                    cerr << "Finished system " << this->count << ": " << this->handler.system.id64 << "(" << this->handler.system.x << "," << this->handler.system.y << "," << this->handler.system.z << ") (";
                    if (this->handler.system.isNeutron) {
                        cerr << "neutron/";
                    } else {
                        cerr << "no neutron/";
                    }
                    if (this->handler.system.isScoopable) {
                        cerr << "is scoopable/";
                    } else {
                        cerr << "not scoopable/";
                    }
                    if (this->handler.system.isPristine) {
                        cerr << "is pristine/";
                    } else {
                        cerr << "not pristine/";
                    }
                    if (this->handler.system.hasIcyRing) {
                        cerr << "has icy ring";
                    } else {
                        cerr << "no icy ring";
                    }
                    cerr << ") in " << last_time_span.count() << " seconds at " << (REPORT_SIZE / last_time_span.count()) << "/sec, total " << total_time_span.count() << " seconds at " << (count / total_time_span.count()) << "/sec" << endl;

                    this->t3 = this->t2;
                }

                // cerr << this->handler.system.id64 << "(" << this->handler.system.x << "," << this->handler.system.y << "," << this->handler.system.z << ") " << endl;

                double coords[] = {this->handler.system.x,this->handler.system.y,this->handler.system.z};
                Region region(coords, coords, 3);
                this->m_pNext = new RTree::Data(sizeof(System), reinterpret_cast<uint8_t*>(&this->handler.system), region, this->handler.system.id64);
                return;
            }
        }
        if (reader.HasParseError()) {
            cerr << "Parse error in " <<
                filename << " [" <<
                GetParseError_En(reader.GetParseErrorCode()) <<
                "] offset [" <<
                reader.GetErrorOffset() <<
                "]" <<
                endl;
            throw "Parse error";
        }
    
    }

    RTree::Data* m_pNext;
};


int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <galaxy filename> <output_filename>" << endl;
        exit(1);
    }
    
    char* pszVersion = SIDX_Version();
    cerr << "libspatialindex version: " << pszVersion << endl;
    free(pszVersion);

    MyDataStream stream(argv[1]);

    string filename = argv[2];
    IStorageManager* diskfile = StorageManager::createNewDiskStorageManager(filename, 4096);
    StorageManager::IBuffer* file = StorageManager::createNewRandomEvictionsBuffer(*diskfile, 10, false);

    id_type indexIdentifier;

    ISpatialIndex* tree = RTree::createAndBulkLoadNewRTree(
        RTree::BLM_STR, stream, *file, 0.9, 64, 1024*64, 3, SpatialIndex::RTree::RV_RSTAR, indexIdentifier
    );
    tree->flush();
    file->flush();
    diskfile->flush();

    IStatistics* stats;
    tree->getStatistics(&stats);
    cerr << "Nodes: " << stats->getNumberOfNodes() << " Data: " << stats->getNumberOfData() << endl;
    delete stats;

    delete diskfile;

    cerr << "Index identifier: " << indexIdentifier << endl;

}
