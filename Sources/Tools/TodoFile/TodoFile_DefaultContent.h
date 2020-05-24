#ifndef CTODOFILE_DEFAULTCONTENT_H
#define CTODOFILE_DEFAULTCONTENT_H

#define DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_0_ "Parameters"
#define DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_0_LogfilePath "LogFile"

#define DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_1_ "To Do List"

const TS08_t Default_ToDoFile_Content = \
";To Do List\n\
;How To Use?\n\
;Recognized commands are : Copy, Move, Delete, Launch\n\
;Samples:\n\
;Copy=Source Directory > Target Directory\n\
;Copy=Source File > Target File\n\
;Move=Source Directory > Target Directory\n\
;Move=Source File > Target File\n\
;Delete=Directory\n\
;Delete=File\n\
;Launch=FilePath\n\
["DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_0_"]\n\
;"DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_0_LogfilePath"=./log.txt \n\
"DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_0_LogfilePath"=\n\
["DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_1_"]\n\
";
#endif