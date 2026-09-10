#include "Transcript.h"
#include <cstring>

Transcript::Transcript()
{
    reset();
};

Transcript::Transcript(const Transcript& t) {
    *this = t;
}

Transcript& Transcript::operator=(const Transcript& t) {
    if (this == &t) return *this;
    // Scalars / pointers (shallow for readName etc., matching default copy)
    nExons = t.nExons;
    readLengthOriginal = t.readLengthOriginal;
    readLength = t.readLength;
    Lread = t.Lread;
    readLengthPairOriginal = t.readLengthPairOriginal;
    iRead = t.iRead;
    readNmates = t.readNmates;
    readName = t.readName;
    iFrag = t.iFrag;
    rStart = t.rStart; roStart = t.roStart; rLength = t.rLength;
    gStart = t.gStart; gLength = t.gLength; cStart = t.cStart;
    Chr = t.Chr; Str = t.Str; roStr = t.roStr;
    haploType = t.haploType;
    primaryFlag = t.primaryFlag;
    nMatch = t.nMatch; nMM = t.nMM; mappedLength = t.mappedLength;
    extendL = t.extendL; maxScore = t.maxScore;
    nGap = t.nGap; lGap = t.lGap; nDel = t.nDel; nIns = t.nIns; lDel = t.lDel; lIns = t.lIns;
    nUnique = t.nUnique; nAnchor = t.nAnchor;
    intronMotifs[0] = t.intronMotifs[0];
    intronMotifs[1] = t.intronMotifs[1];
    intronMotifs[2] = t.intronMotifs[2];
    sjMotifStrand = t.sjMotifStrand;
    sjYes = t.sjYes;
    // Only copy used exon / SJ slots (nExons exons; nExons-1 junctions when nExons>0)
    if (nExons > 0) {
        std::memcpy(exons, t.exons, nExons * sizeof(exons[0]));
        if (nExons > 1) {
            const uint nJ = nExons - 1;
            std::memcpy(shiftSJ, t.shiftSJ, nJ * sizeof(shiftSJ[0]));
            std::memcpy(canonSJ, t.canonSJ, nJ * sizeof(canonSJ[0]));
            std::memcpy(sjAnnot, t.sjAnnot, nJ * sizeof(sjAnnot[0]));
            std::memcpy(sjStr, t.sjStr, nJ * sizeof(sjStr[0]));
        }
    }
    // Skip container churn when both empty (common during stitch forks).
    if (!t.varInd.empty() || !varInd.empty()) {
        if (t.varInd.empty()) varInd.clear(); else varInd = t.varInd;
    }
    if (!t.varGenCoord.empty() || !varGenCoord.empty()) {
        if (t.varGenCoord.empty()) varGenCoord.clear(); else varGenCoord = t.varGenCoord;
    }
    if (!t.varReadCoord.empty() || !varReadCoord.empty()) {
        if (t.varReadCoord.empty()) varReadCoord.clear(); else varReadCoord = t.varReadCoord;
    }
    if (!t.varAllele.empty() || !varAllele.empty()) {
        if (t.varAllele.empty()) varAllele.clear(); else varAllele = t.varAllele;
    }
    if (!t.alignGenes.empty() || !alignGenes.empty()) {
        if (t.alignGenes.empty()) alignGenes.clear(); else alignGenes = t.alignGenes;
    }
    if (!t.cigar.empty() || !cigar.empty()) {
        if (t.cigar.empty()) cigar.clear(); else cigar = t.cigar;
    }
    return *this;
}




void Transcript::copyStitchCore(const Transcript& t) {
    // Hot stitch fork: copy state used by stitchWindowAligns / stitchAlign / extend / finalize
    // without touching var*/alignGenes/cigar (empty during stitch recursion).
    nExons = t.nExons;
    readLengthOriginal = t.readLengthOriginal;
    readLength = t.readLength;
    Lread = t.Lread;
    readLengthPairOriginal = t.readLengthPairOriginal;
    iRead = t.iRead;
    readNmates = t.readNmates;
    readName = t.readName;
    iFrag = t.iFrag;
    rStart = t.rStart; roStart = t.roStart; rLength = t.rLength;
    gStart = t.gStart; gLength = t.gLength; cStart = t.cStart;
    Chr = t.Chr; Str = t.Str; roStr = t.roStr;
    haploType = t.haploType;
    primaryFlag = t.primaryFlag;
    nMatch = t.nMatch; nMM = t.nMM; mappedLength = t.mappedLength;
    extendL = t.extendL; maxScore = t.maxScore;
    nGap = t.nGap; lGap = t.lGap; nDel = t.nDel; nIns = t.nIns; lDel = t.lDel; lIns = t.lIns;
    nUnique = t.nUnique; nAnchor = t.nAnchor;
    intronMotifs[0] = t.intronMotifs[0];
    intronMotifs[1] = t.intronMotifs[1];
    intronMotifs[2] = t.intronMotifs[2];
    sjMotifStrand = t.sjMotifStrand;
    sjYes = t.sjYes;
    if (nExons > 0) {
        std::memcpy(exons, t.exons, nExons * sizeof(exons[0]));
        if (nExons > 1) {
            const uint nJ = nExons - 1;
            std::memcpy(shiftSJ, t.shiftSJ, nJ * sizeof(shiftSJ[0]));
            std::memcpy(canonSJ, t.canonSJ, nJ * sizeof(canonSJ[0]));
            std::memcpy(sjAnnot, t.sjAnnot, nJ * sizeof(sjAnnot[0]));
            std::memcpy(sjStr, t.sjStr, nJ * sizeof(sjStr[0]));
        }
    }
}

void Transcript::reset() {
    extendL=0;

//     for (uint ii=0;ii<4;ii++) {
//         polyXlength[ii]=0;
//         polyXnMM[ii]=0;
//     };
    primaryFlag=false;

    rStart=0; roStart=0; rLength=0; gStart=0; gLength=0; //read and genomic coordinates

    maxScore=0;
    nMatch=0;
    nMM=0;

    nGap=0; lGap=0; lDel=0; lIns=0; nDel=0; nIns=0;

    nUnique=nAnchor=0;
};

void Transcript::add(Transcript *trIn) {
    maxScore+=trIn->maxScore;
    nMatch+=trIn->nMatch;
    nMM+=trIn->nMM;
    nGap+=trIn->nGap; lGap+=trIn->lGap;
    lDel+=trIn->lDel; nDel+=trIn->nDel;
    lIns+=trIn->lIns; nIns+=trIn->nIns;
    nUnique+=trIn->nUnique;
};

void Transcript::extractSpliceJunctions(vector<array<uint64,2>> &sjOut, bool &annotYes)
{
    annotYes=true;
    for (uint64 iex=0; iex<nExons-1; iex++) {//record all junctions
        if (canonSJ[iex]>=0) {//only record junctions, not indels or mate gap
            array<uint64,2> sj;
            sj[0]=exons[iex][EX_G]+exons[iex][EX_L];//start
            sj[1]=exons[iex+1][EX_G] - sj[0]; //gap
            sjOut.push_back(sj);
            if (sjAnnot[iex]==0)
                annotYes=false;//if one of the SJs is unannoated, annotYes=false
        };
    };
};

uint64 Transcript::chrStartLengthExtended()
{
    uint64 start1  = cStart - exons[0][EX_R];
    uint64 length1 = exons[nExons-1][EX_G] + Lread - exons[nExons-1][EX_R] - exons[0][EX_G] + exons[0][EX_R];
    
    return (start1 << 32) | length1;
};
