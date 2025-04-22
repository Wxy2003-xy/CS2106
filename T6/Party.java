/**
 * 
 * is_sanctioned(PId).
*/

import java.util.*;



// Test cases:
// Parties
// {partyId=P001, jurisdiction=EU, clientType=Retail, isSanctioned=false}
// {partyId=P002, jurisdiction=EU, clientType=Institutional, isSanctioned=false}
// {partyId=P003, jurisdiction=US, clientType=Retail, isSanctioned=true}
// {partyId=P010, jurisdiction=SG, clientType=Institutional, isSanctioned=false}

// Instruments
// {instrumentId=I001, instrumentType=Derivatives, allowedJurisdiction=EU}
// {instrumentId=I002, instrumentType=Bonds, allowedJurisdiction=US,EU}
// {instrumentId=I003, instrumentType=Stocks, allowedJurisdiction=EU,SG}
// {instrumentId=I004, instrumentType=Forex, allowedJurisdiction=EU,US}

// InstrumentPermissions
// {Institutional=Derivatives,Bonds,Stocks,Forex}
// {Retail=Bonds,Stocks}

// isGoodToTrade(P001, I002) => true
// isGoodToTrade(P003, I004) => false (retail clients can only trade bonds and stocks)
// isGoodToTrade(P010, I002) => false (I002 is not allowed in SG)

// Code Skeleton
// static class Party {
// String id, jurisdiction, clientType;
// boolean isSanctioned;

// Party(String id, String jurisdiction, String clientType, boolean isSanctioned) {
// this.id = id;
// this.jurisdiction = jurisdiction;
// this.clientType = clientType;
// this.isSanctioned = isSanctioned;
// }
// }

// static class Instrument {
// String id, type;
// Set<String> allowedJurisdictions;

// Instrument(String id, String type, Set<String> allowedJurisdictions) {
// this.id = id;
// this.type = type;
// this.allowedJurisdictions = allowedJurisdictions;
//       }
// }
    
static class Instrument {
      String id, type;
      Set<String> allowedJurisdictions;
      
      Instrument(String id, String type, Set<String> allowedJurisdictions) {
      this.id = id;
      this.type = type;
      this.allowedJurisdictions = allowedJurisdictions;
      }
  }

static class Party {
    String id, jurisdiction, clientType;
    boolean isSanctioned;
    
    Party(String id, String jurisdiction, String clientType, boolean isSanctioned) {
    this.id = id;
    this.jurisdiction = jurisdiction;
    this.clientType = clientType;
    this.isSanctioned = isSanctioned;
    }

    public static boolean can_trade(Party p, Instrument I) {
       if (p.isSanctioned) {
            return false;
       }
       String t = I.type;
       Set allowedjur = I.allowedJurisdictions
       if (!allowedjur.contains(p.jurisdiction)) {
            return fasle;
       }
       if (!p.clientType.contains(I.instrumentType)) {
            return fasle;
       }
    }

    public static boolean f(Party p, Instrument I) {
      return 
    }
}

    


