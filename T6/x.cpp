#include<bits/stdc++.h>
using namespace std;
class Instrument {
      string id, type;
      set<string> allowedJurisdictions;

      
      Instrument(string id, string type, set<string> allowedJurisdictions) {
        id = id;
        type = type;
        allowedJurisdictions = allowedJurisdictions;
      }
      public:
      set<string> get_j() {
        return allowedJurisdictions;
      }
      string get_type() {
        return type;
      }

  };


class Party {
    string id, jurisdiction, clientType;
    bool isSanctioned;
    
    Party(string id, string jurisdiction, string clientType, bool isSanctioned) {
        id = id;
        jurisdiction = jurisdiction;
        clientType = clientType;
        isSanctioned = isSanctioned;
    }

    bool allowed_to_trade(Party p, Instrument i) {
        if (p.isSanctioned) return false;
        set<string> allowed_jur = i.get_j();
        if (!allowed_jur.count(p.jurisdiction)) {
            return false;
        }
        string itype = i.get_type();
        string t1[4] = {"Derivatives","Bonds","Stocks","Forex"};
        set<string> a(t1, t1+4);
        string t2[2] = {"Bonds","Stocks"};
        set<string> b(t2, t2+4);
        unordered_map<string, set<string>> instr_perm({{"Institutional", a}, {"Retail", b}});
        set<string> perm_type = instr_perm.find(p.clientType);
        if (!perm_type.count(itype)) return false;
        return true;
    }
};