#ifndef MARCRELATOR_H
#define MARCRELATOR_H

#include <QString>

class MarcRelator
{
public:
  enum Relator {
    NO_TYPE = 0,
    ABRIDGER,
    ART_COPYIST,
    ACTOR,
    ART_DIRECTOR,
    ADAPTER,
    AUTHOR_OF_AFTERWORD,
    ANALYST,
    ANIMATOR,
    ANNOTATOR,
    BIBLIOGRAPHIC_ANTECEDENT,
    APPELLEE,
    APPELLANT,
    APPLICANT,
    AUTHOR_IN_QUOTATIONS,
    ARCHITECT,
    ARTISTIC_DIRECTOR,
    ARRANGER,
    ARTIST,
    ASSIGNEE,
    ASSOCIATED_NAME,
    AUTOGRAPHER,
    ATTRIBUTED_NAME,
    AUCTIONEER,
    AUTHOR_OF_DIALOG,
    AUTHOR_OF_INTRODUCTION,
    SCREENWRITER,
    AUTHOR,
    BINDING_DESIGNER,
    BOOKJACKET_DESIGNER,
    BOOK_DESIGNER,
    BOOK_PRODUCER,
    BLURB_WRITER,
    BINDER,
    BOOKPLATE_DESIGNER,
    BROADCASTER,
    BRAILE_EMBOSSER,
    BOOKSELLER,
    CASTER,
    CONCEPTOR,
    CHOREOGRAPHER,
    COLLABORATOR, // Discontinued
    CLIENT,
    CALLIGRAPHER,
    COLORIST,
    COLLOTYPER,
    COMMENTATOR,
    COMPOSER,
    COMPOSITOR,
    CONDUCTOR,
    CINEMATOGRAPHER,
    CENSOR,
    CONTESTANT_APPELLEE,
    CONTESTOR,
    COMPILER,
    CONSERVATOR,
    COLLECTION_REGISTRAR,
    CONTESTANT,
    CONTESTANT_APPELLANT,
    COURT_GOVERNED,
    COVER_DESIGNER,
    COPYRIGHT_CLAIMANT,
    COMPLAINANT_APPELLEE,
    COPYRIGHT_HOLDER,
    COMPLAINANT,
    COMPLAINANT_APPELLANT,
    CREATOR,
    CORRESPONDANT,
    CORRECTOR,
    COURT_REPORTER,
    CONSULTANT,
    CONSULTANT_TO_A_PROJECT,
    COSTUME_DESIGNER,
    CONTRIBUTOR,
    CONTESTEE_APPELLEE,
    CARTOGRAPHER,
    CONTRACTOR,
    CONTESTEE,
    CONTESTEE_APPELLANT,
    CURATOR,
    COMMENTATOR_FOR_WRITTEN_TEXT,
    DISTIBUTION_PLACE,
    DEFENDANT,
    DEFENDANT_APPELLEE,
    DEFENDANT_APPELLANT,
    DEGREE_GRANTING_INSTITUTION,
    DEGREE_SUPERVISOR,
    DISSERTANT,
    DELINEATOR,
    DANCER,
    DONOR,
    DEPICTED,
    DEPOSITOR,
    DRAFTSMAN,
    DIRECTOR,
    DESIGNER,
    DISTRIBUTOR,
    DATA_CONTRIBUTOR,
    DEDICATEE,
    DATA_MANAGER,
    DEDICATOR,
    DUBIOUS_AUTHOR,
    EDITOR_OF_COMPILATION,
    EDITOR_OF_MOVING_IMAGE_WORK,
    EDITOR,
    ENGRAVER,
    ELECTRICIAN,
    ELECTROTYPER,
    ENGINEER,
    ENACTING_JURISTICTION,
    ETCHER,
    EVENT_PLACE,
    EXPERT,
    FACSIMILIST,
    FILM_DISTRIBUTOR,
    FIELD_DIRECTOR,
    FILM_EDITOR,
    FILM_DIRECTOR,
    FILM_MAKER,
    FORMER_OWNOR,
    FILM_PRODUCER,
    FUNDER,
    FIRST_PARTY,
    FORGER,
    GEOGRAPHIC_INFORMATION_SPECIALIST,
    GRAPHIC_TECHNICIAN, // Discontinued
    HOST_INSTITUTION,
    HONOREE,
    HOST,
    ILLISTRATOR,
    ILLUMINATOR,
    INSCRIBER,
    INVENTOR,
    ISSUING_BODY,
    INSTRUMENTALIST,
    INTERVIEWEE,
    INTERVIEWER,
    JUDGE,
    JURISTICTION_GOVERNED,
    LABORATORY,
    LIBRETTIST,
    LABORATORY_DIRECTOR,
    LEAD,
    LIBELLEE_APPELEE,
    LIBELLEE,
    LENDER,
    LIBELLEE_APPELLANT,
    LIGHTING_DESIGNER,
    LIBELANT_APPELLEE,
    LIBELLANT,
    LIBELANT_APPELLANT,
    LANDSCAPE_ARCHITECT,
    LICENSEE,
    LICENSOR,
    LITHOGRAPHER,
    LYRICIST,
    MUSIC_COPYIST,
    METADATA_CONTACT,
    MEDIUM,
    MANUFACTURE_PLACE,
    MANFACTURER,
    MODERATOR,
    MONITOR,
    MARBLER,
    MARKUP_EDITOR,
    MUSICAL_DIRECTOR,
    METAL_ENGRAVER,
    MINUTE_TAKER,
    MUSICIAN,
    NARRATOR,
    OPPONENT,
    ORIGINATOR,
    ORGANISER,
    ONSCREEN_PRESENTER,
    OTHER,
    OWNER,
    PANELIST,
    PATRON,
    PUBLISHING_DIRECTOR,
    PUBLISHER,
    PROJECT_DIRECTOR,
    PROOFREADER,
    PHOTOGRAPHER,
    PLATEMAKER,
    PERMIITIN_AGENCY,
    PRODUCTION_MANAGER,
    PRINTER_OF_PLATES,
    PAPERMAKE,
    PUPPETEER,
    PRAESES,
    PROCESS_CONTACT,
    PRODUCTION_PERSONNAL,
    PRESENTER,
    PERFORMER,
    PROGRAMMER,
    PRINTMAKER,
    PRODUCTION_COMPANY,
    PRODUCER,
    PRODUCTION_PLACE,
    PRODUCTION_DESIGNER,
    PRINTER,
    PROVIDER,
    PATENT_APPLICATION,
    PLAINTIFF_APPELLEE,
    PLAINTIFF,
    PATENT_HOLDER,
    PLAINTIFF_APPELLANT,
    PUBLICATION_PLACE,
    RUBRICATOR,
    RECORDIST,
    RECORDING_ENGINEER,
    ADDRESSEE,
    RADIO_DIRECTOR,
    REDAKTOR,
    RENDERER,
    RESEARCHER,
    REVIEWER,
    RADIO_PRODUCER,
    REPOSITORY,
    REPOSRTER,
    RESPONSIBLE_PARTY,
    RESPONDANT_APPELLEE,
    RESTAGER,
    RESPONDANT,
    RESTORATIONIST,
    RESPONDANT_APPELLANT,
    RESEARCH_TEAM_HEAD,
    RESEARCH_TEAM_MEMBER,
    SCIENTIFIC_ADVISOR,
    SCENARIST,
    SCULPTOR,
    SCRIBE,
    SOUND_DESIGNER,
    SECRETARY,
    STAGE_DIRECTOR,
    SIGNER,
    SUPPORTING_HOST,
    SELLER,
    SINGER,
    SPEAKER,
    SPONSOR,
    SECOND_PARTY,
    SURVEYOR,
    SET_DESIGNER,
    SETTING,
    STORYTELLER,
    STAGE_MANAGER,
    STANDARDS_BODY,
    STEREOTYPER,
    TECHNICAL_DIRECTOR,
    TEACHER,
    THESIS_ADVISOR,
    TELEVISION_DIRECTOR,
    TELEVISION_PRODUCER,
    TRANSCRIBER,
    TRANSLATOR,
    TYPE_DESIGNER,
    TYPOGRAPHER,
    UNIVERSITY_PLACE,
    VOICE_ACTOR,
    VIDEOGRAPHER,
    VOCALIST, // Discontinued
    WRITER_OF_ADDED_COMMENTARY,
    WRITER_OF_ADDED_LYRICS,
    WRITER_OF_ACCOMPANYING_MATERIAL,
    WRITER_OF_ADDED_TEXT,
    WOODCUTTER,
    WOOD_ENGRAVER,
    WRITER_OF_INTRODUCTION,
    WITNESS,
    WRITER_OF_PREFACE,
    WRITER_OF_SUPPLEMENTARY_TEXTUAL_CONTENT,
  };

  MarcRelator();

  Relator type();
  void setType(Relator type);

  QString code();
  QString asString();
  bool isRelator();

  static QString toString(Relator relator);
  static MarcRelator fromString(QString relator_name);

  static bool isRelator(QString name);

protected:
  Relator m_type;
  QString m_code;

  void setCode(QString code);
};

#endif // MARCRELATOR_H

// From the MARC:Relator code list
// <http://www.loc.gov/marc/relators/relacode.html> For more information see
// <http://www.loc.gov/marc/relators/relaterm.html>

// abr 	Abridger
// acp 	Art copyist
// act 	Actor
// adi 	Art director
// adp 	Adapter
// aft 	Author of afterword, colophon, etc.
// anl 	Analyst
// anm 	Animator
// ann 	Annotator
// ant 	Bibliographic antecedent
// ape 	Appellee
// apl 	Appellant
// app 	Applicant
// aqt 	Author in quotations or text abstracts
// arc 	Architect
// ard 	Artistic director
// arr 	Arranger
// art 	Artist
// asg 	Assignee
// asn 	Associated name
// ato 	Autographer
// att 	Attributed name
// auc 	Auctioneer
// aud 	Author of dialog
// aui 	Author of introduction, etc.
// aus 	Screenwriter
// aut 	Author
// bdd 	Binding designer
// bjd 	Bookjacket designer
// bkd 	Book designer
// bkp 	Book producer
// blw 	Blurb writer
// bnd 	Binder
// bpd 	Bookplate designer
// brd 	Broadcaster
// brl 	Braille embosser
// bsl 	Bookseller
// cas 	Caster
// ccp 	Conceptor
// chr 	Choreographer
//-clb 	Collaborator
// cli 	Client
// cll 	Calligrapher
// clr 	Colorist
// clt 	Collotyper
// cmm 	Commentator
// cmp 	Composer
// cmt 	Compositor
// cnd 	Conductor
// cng 	Cinematographer
// cns 	Censor
// coe 	Contestant-appellee
// col 	Collector
// com 	Compiler
// con 	Conservator
// cor 	Collection registrar
// cos 	Contestant
// cot 	Contestant-appellant
// cou 	Court governed
// cov 	Cover designer
// cpc 	Copyright claimant
// cpe 	Complainant-appellee
// cph 	Copyright holder
// cpl 	Complainant
// cpt 	Complainant-appellant
// cre 	Creator
// crp 	Correspondent
// crr 	Corrector
// crt 	Court reporter
// csl 	Consultant
// csp 	Consultant to a project
// cst 	Costume designer
// ctb 	Contributor
// cte 	Contestee-appellee
// ctg 	Cartographer
// ctr 	Contractor
// cts 	Contestee
// ctt 	Contestee-appellant
// cur 	Curator
// cwt 	Commentator for written text
// dbp 	Distribution place
// dfd 	Defendant
// dfe 	Defendant-appellee
// dft 	Defendant-appellant
// dgg 	Degree granting institution
// dgs 	Degree supervisor
// dis 	Dissertant
// dln 	Delineator
// dnc 	Dancer
// dnr 	Donor
// dpc 	Depicted
// dpt 	Depositor
// drm 	Draftsman
// drt 	Director
// dsr 	Designer
// dst 	Distributor
// dtc 	Data contributor
// dte 	Dedicatee
// dtm 	Data manager
// dto 	Dedicator
// dub 	Dubious author
// edc 	Editor of compilation
// edm 	Editor of moving image work
// edt 	Editor
// egr 	Engraver
// elg 	Electrician
// elt 	Electrotyper
// eng 	Engineer
// enj 	Enacting jurisdiction
// etr 	Etcher
// evp 	Event place
// exp 	Expert
// fac 	Facsimilist
// fds 	Film distributor
// fld 	Field director
// flm 	Film editor
// fmd 	Film director
// fmk 	Filmmaker
// fmo 	Former owner
// fmp 	Film producer
// fnd 	Funder
// fpy 	First party
// frg 	Forger
// gis 	Geographic information specialist
//-grt 	Graphic technician
// his 	Host institution
// hnr 	Honoree
// hst 	Host
// ill 	Illustrator
// ilu 	Illuminator
// ins 	Inscriber
// inv 	Inventor
// isb 	Issuing body
// itr 	Instrumentalist
// ive 	Interviewee
// ivr 	Interviewer
// jud 	Judge
// jug 	Jurisdiction governed
// lbr 	Laboratory
// lbt 	Librettist
// ldr 	Laboratory director
// led 	Lead
// lee 	Libelee-appellee
// lel 	Libelee
// len 	Lender
// let 	Libelee-appellant
// lgd 	Lighting designer
// lie 	Libelant-appellee
// lil 	Libelant
// lit 	Libelant-appellant
// lsa 	Landscape architect
// lse 	Licensee
// lso 	Licensor
// ltg 	Lithographer
// lyr 	Lyricist
// mcp 	Music copyist
// mdc 	Metadata contact
// med 	Medium
// mfp 	Manufacture place
// mfr 	Manufacturer
// mod 	Moderator
// mon 	Monitor
// mrb 	Marbler
// mrk 	Markup editor
// msd 	Musical director
// mte 	Metal-engraver
// mtk 	Minute taker
// mus 	Musician
// nrt 	Narrator
// opn 	Opponent
// org 	Originator
// orm 	Organizer
// osp 	Onscreen presenter
// oth 	Other
// own 	Owner
// pan 	Panelist
// pat 	Patron
// pbd 	Publishing director
// pbl 	Publisher
// pdr 	Project director
// pfr 	Proofreader
// pht 	Photographer
// plt 	Platemaker
// pma 	Permitting agency
// pmn 	Production manager
// pop 	Printer of plates
// ppm 	Papermaker
// ppt 	Puppeteer
// pra 	Praeses
// prc 	Process contact
// prd 	Production personnel
// pre 	Presenter
// prf 	Performer
// prg 	Programmer
// prm 	Printmaker
// prn 	Production company
// pro 	Producer
// prp 	Production place
// prs 	Production designer
// prt 	Printer
// prv 	Provider
// pta 	Patent applicant
// pte 	Plaintiff-appellee
// ptf 	Plaintiff
// pth 	Patent holder
// ptt 	Plaintiff-appellant
// pup 	Publication place
// rbr 	Rubricator
// rcd 	Recordist
// rce 	Recording engineer
// rcp 	Addressee
// rdd 	Radio director
// red 	Redaktor
// ren 	Renderer
// res 	Researcher
// rev 	Reviewer
// rpc 	Radio producer
// rps 	Repository
// rpt 	Reporter
// rpy 	Responsible party
// rse 	Respondent-appellee
// rsg 	Restager
// rsp 	Respondent
// rsr 	Restorationist
// rst 	Respondent-appellant
// rth 	Research team head
// rtm 	Research team member
// sad 	Scientific advisor
// sce 	Scenarist
// scl 	Sculptor
// scr 	Scribe
// sds 	Sound designer
// sec 	Secretary
// sgd 	Stage director
// sgn 	Signer
// sht 	Supporting host
// sll 	Seller
// sng 	Singer
// spk 	Speaker
// spn 	Sponsor
// spy 	Second party
// srv 	Surveyor
// std 	Set designer
// stg 	Setting
// stl 	Storyteller
// stm 	Stage manager
// stn 	Standards body
// str 	Stereotyper
// tcd 	Technical director
// tch 	Teacher
// ths 	Thesis advisor
// tld 	Television director
// tlp 	Television producer
// trc 	Transcriber
// trl 	Translator
// tyd 	Type designer
// tyg 	Typographer
// uvp 	University place
// vac 	Voice actor
// vdg 	Videographer
//-voc 	Vocalist
// wac 	Writer of added commentary
// wal 	Writer of added lyrics
// wam 	Writer of accompanying material
// wat 	Writer of added text
// wdc 	Woodcutter
// wde 	Wood engraver
// win 	Writer of introduction
// wit 	Witness
// wpr 	Writer of prefacebdd 	Binding designer
// wst 	Writer of supplementary textual content
