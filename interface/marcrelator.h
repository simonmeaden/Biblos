#ifndef MARCRELATOR_H
#define MARCRELATOR_H

#include <QString>

class MarcRelator
{
public:
  enum Relator {
    no_type = 0,
    Abridger,
    ArtCopyist,
    Actor,
    ArtDirector,
    Adapter,
    AuthorOfAfterword,
    Analyst,
    Animator,
    Annotator,
    BibliographicAntecedent,
    Appellee,
    Appellant,
    Applicant,
    AuthorInQuotations,
    Architect,
    ArtisticDirector,
    Arranger,
    Artist,
    Assignee,
    AssociatedName,
    Autographer,
    AttributedName,
    Auctioneer,
    AuthorOfDialog,
    AuthorOfIntroduction,
    Screenwriter,
    Author,
    BindingDesigner,
    BookjacketDesigner,
    BookDesigner,
    BookProducer,
    BlurbWriter,
    Binder,
    BookplateDesigner,
    Broadcaster,
    BrailleEmbosser,
    Bookseller,
    Caster,
    Conceptor,
    Choreographer,
    Collaborator, // Discontinued
    Client,
    Calligrapher,
    Colorist,
    Collotyper,
    Commentator,
    Composer,
    Compositor,
    Conductor,
    Cinematographer,
    Censor,
    ContestantAppellee,
    Collector,
    Compiler,
    Conservator,
    CollectionRegistrar,
    Contestant,
    ContestantAppellant,
    CourtGoverned,
    CoverDesigner,
    CopyrightClaimant,
    ComplainantAppellee,
    CopyrightHolder,
    Complainant,
    ComplainantAppellant,
    Creator,
    Correspondent,
    Corrector,
    CourtReporter,
    Consultant,
    ConsultantToAProject,
    CostumeDesigner,
    Contributor,
    ContesteeAppellee,
    Cartographer,
    Contractor,
    Contestee,
    ContesteeAppellant,
    Curator,
    CommentatorForWrittenText,
    DistributionPlace,
    Defendant,
    DefendantAppellee,
    DefendantAppellant,
    DegreeGrantingInstitution,
    DegreeSupervisor,
    Dissertant,
    Delineator,
    Dancer,
    Donor,
    Depicted,
    Depositor,
    Draftsman,
    Director,
    Designer,
    Distributor,
    DataContributor,
    Dedicatee,
    DataManager,
    Dedicator,
    DubiousAuthor,
    EditorOfCompilation,
    EditorOfMovingImageWork,
    Editor,
    Engraver,
    Electrician,
    Electrotyper,
    Engineer,
    EnactingJurisdiction,
    Etcher,
    EventPlace,
    Expert,
    Facsimilist,
    FilmDistributor,
    FieldDirector,
    FilmEditor,
    FilmDirector,
    Filmmaker,
    FormerOwner,
    FilmProducer,
    Funder,
    FirstParty,
    Forger,
    GeographicInformationSpecialist,
    GraphicTechnician, // Discontinued
    HostInstitution,
    Honoree,
    Host,
    Illustrator,
    Illuminator,
    Inscriber,
    Inventor,
    IssuingBody,
    Instrumentalist,
    Interviewee,
    Interviewer,
    Judge,
    JurisdictionGoverned,
    Laboratory,
    Librettist,
    LaboratoryDirector,
    Lead,
    LibeleeAppellee,
    Libelee,
    Lender,
    LibeleeAppellant,
    LightingDesigner,
    LibelantAppellee,
    Libelant,
    LibelantAppellant,
    LandscapeArchitect,
    Licensee,
    Licensor,
    Lithographer,
    Lyricist,
    MusicCopyist,
    MetadataContact,
    Medium,
    ManufacturePlace,
    Manufacturer,
    Moderator,
    Monitor,
    Marbler,
    MarkupEditor,
    MusicalDirector,
    MetalEngraver,
    MinuteTaker,
    Musician,
    Narrator,
    Opponent,
    Originator,
    Organizer,
    OnscreenPresenter,
    Other,
    Owner,
    Panelist,
    Patron,
    PublishingDirector,
    Publisher,
    ProjectDirector,
    Proofreader,
    Photographer,
    Platemaker,
    PermittingAgency,
    ProductionManager,
    PrinterOfPlates,
    Papermaker,
    Puppeteer,
    Praeses,
    ProcessContact,
    ProductionPersonnel,
    Presenter,
    Performer,
    Programmer,
    Printmaker,
    ProductionCompany,
    Producer,
    ProductionPlace,
    ProductionDesigner,
    Printer,
    Provider,
    PatentApplicant,
    PlaintiffAppellee,
    Plaintiff,
    PatentHolder,
    PlaintiffAppellant,
    PublicationPlace,
    Rubricator,
    Recordist,
    RecordingEngineer,
    Addressee,
    RadioDirector,
    Redaktor,
    Renderer,
    Researcher,
    Reviewer,
    RadioProducer,
    Repository,
    Reporter,
    ResponsibleParty,
    RespondentAppellee,
    Restager,
    Respondent,
    Restorationist,
    RespondentAppellant,
    ResearchTeamHead,
    ResearchTeamMember,
    ScientificAdvisor,
    Scenarist,
    Sculptor,
    Scribe,
    SoundDesigner,
    Secretary,
    StageDirector,
    Signer,
    SupportingHost,
    Seller,
    Singer,
    Speaker,
    Sponsor,
    SecondParty,
    Surveyor,
    SetDesigner,
    Setting,
    Storyteller,
    StageManager,
    StandardsBody,
    Stereotyper,
    TechnicalDirector,
    Teacher,
    ThesisAdvisor,
    TelevisionDirector,
    TelevisionProducer,
    Transcriber,
    Translator,
    TypeDesigner,
    Typographer,
    UniversityPlace,
    VoiceActor,
    Videographer,
    Vocalist, // Discontinued
    WriterOfAddedCommentary,
    WriterOfAddedLyrics,
    WriterOfAccompanyingMaterial,
    WriterOfAddedText,
    Woodcutter,
    WoodEngraver,
    WriterOfIntroduction,
    Witness,
    WriterOfPreface,
    WriterOfSupplementaryTextualContent,
  };

  MarcRelator();

  Relator type();
  void setType(Relator type);

  QString asString();

  static QString toString(Relator relator);
  static MarcRelator fromString(QString name);

protected:
  Relator m_type;
};

#endif // MARCRELATOR_H

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
