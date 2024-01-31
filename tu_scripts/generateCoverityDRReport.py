import sys
import os
import datetime as dt
import json


#================================================================================================
# Setting Variable
#================================================================================================
COVERITY_DIR = ""
MAKE_HTML_TABLE = True


#================================================================================================
# Global Variable
#================================================================================================
PRINT_DEBUG = False
MAKE_SEPERTATED_REPORT = False

SEPERTATED_REPORT_DIR = "reports"
HTML_TABLE_DIR = "tables"

REPORT_TITLE_STRING = "Full File Path, Type, Comment Line, Issue, File, Checker, Statement Line, Statement, Function, Cause of Issue"

COVERITY_PREVIEWREPORT_FILE = "/output/preview-report.json"

DEFAULT_COV_DIR="_cov_results_"
TARGETFILE_EXTENSION_LIST = [".c",".cpp",".h"]
COVERITY_COMMNET_LIST = ["/* coverity","// coverity"]
SKIP_STATEMENT_LIST = ["/*","//"]

INPUT_DIRECTORY = ""
TIME_STR = ""

#================================================================================================
# Function
#================================================================================================
def checkInputArg():
    if len(sys.argv) == 1:
        INPUT_DIRECTORY = os.getcwd()
    elif len(sys.argv) == 2:
        INPUT_DIRECTORY = sys.argv[1]
    else:
        print("[ERROR] Invalid input argument")
        sys.exit()

    # check directory
    if not os.path.isdir(INPUT_DIRECTORY):
        print("[ERROR] " + INPUT_DIRECTORY + " is not a directory")
        sys.exit()

    return INPUT_DIRECTORY

def isCoverityIssueFile(inputFile):
    for ext in TARGETFILE_EXTENSION_LIST:
        if inputFile.endswith(ext):
            return True

    return False

def getIssuelist():
    targetFilelist = []
    headerFileslist = []

    fileCnt = 0
    dirCnt = 0
    srcFileCnt = 0
    headFileCnt = 0

    for root, dirs, files in os.walk(INPUT_DIRECTORY):
        for file in files:
            fileCnt += 1

            if isCoverityIssueFile(file):
                targetFilelist.append(root + "/" + file)
                srcFileCnt += 1

        for dir in dirs:
            dirCnt += 1

    if PRINT_DEBUG:
        print("=====================================")
        print("Get Item List Result")
        print("----------------------------")
        print("  - all files count: " + str(fileCnt))
        print("  - all directories count: " + str(dirCnt))
        print("  - source files count: " + str(len(targetFilelist)))
        print("  - source files extensions: " + str(TARGETFILE_EXTENSION_LIST))
        print("=====================================")

    return targetFilelist

def isSkipFile(inputStr):
    for preStr in SKIP_STATEMENT_LIST:
        if inputStr.startswith(preStr):
            return True

    return False

def getStatementString(lineNo, inputLines, inputFile):
    statementStr = ""
    tmpLineNo = lineNo

    for l in inputLines[tmpLineNo:tmpLineNo + 10]:
        l = l.strip()

        if not isSkipFile(l):
            statementStr = l
            break

        tmpLineNo = tmpLineNo + 1

    tmpLineNo = tmpLineNo + 1

    return tmpLineNo, statementStr

def readFileLines(inputfile):
    f = open(inputfile, 'r')
    lines = f.readlines()
    f.close()

    return lines

def writeListToReportFile(inputList, outputfile):
    f = open(outputfile, 'w')
    f.write(REPORT_TITLE_STRING + "\n")
    for item in inputList:
        f.write(item + "\n")
    f.close()

def getCoverityPreviewReportPath():
    reportFile = ""

    if len(COVERITY_DIR) > 2:
        checkingDir = COVERITY_DIR
    else:
        checkingDir = INPUT_DIRECTORY + "/" + DEFAULT_COV_DIR

    reportFile = checkingDir + COVERITY_PREVIEWREPORT_FILE

    if not os.path.isfile(reportFile):
        reportFile = ""

    return reportFile

def checkCoverityResult(fileStr, lineNo):
    causeofIssueStr = ""
    checkerStr = ""
    functionStr = ""

    reportFile = getCoverityPreviewReportPath()

    if len(reportFile) > 2:
        with open(reportFile) as json_file:
            data = json.load(json_file)

            for p in data['issueInfo']:
                for oc in p['occurrences']:
                    if fileStr == oc['file'] and lineNo == (oc['mainEventLineNumber']):
                        checkerStr = oc['checker']
                        causeofIssueStr = oc['mainEventDescription']
                        functionStr = oc['function']

    if len(causeofIssueStr) < 2:
        causeofIssueStr = "COI Unknown"

    if len(checkerStr) < 2:
        checkerStr = "Unknown"

    return functionStr, checkerStr, causeofIssueStr

def makeExcelStr(inputStr):
    inputStr = inputStr.replace(",", "_+_")
    inputStr = inputStr.replace("\"", "\'")
    inputStr = "\"" + inputStr + "\""

    return inputStr

def analyzeFile(inputfile):
    fileIssuelist = []

    if os.path.isfile(inputfile):
        lineNo = 0
        lineList = []

        lines = readFileLines(inputfile)

        for l in lines:
            issueStatement = ""
            statementStr = ""
            issueStr = ""
            typeStr = ""
            causeIssueStr = ""
            checkerStr = ""
            functionStr = ""

            l = l.strip()
            for preStr in COVERITY_COMMNET_LIST:
                if l.startswith(preStr):
                    tmpLineNo = lineNo + 1
                    retLineNo = 0
                    lineList.append(tmpLineNo)

                    issueStr = str(l.split(":")[0].strip()).split("[")[1]
                    typeStr = str(l.split(":")[1].strip()).split("]")[0]

                    retLineNo, statementStr = getStatementString(tmpLineNo, lines, inputfile)
                    shortFile = inputfile.split("/")[-1]

                    statementStr = makeExcelStr(statementStr)

                    functionStr, checkerStr, causeIssueStr = checkCoverityResult(inputfile, retLineNo)
                    causeIssueStr = makeExcelStr(causeIssueStr)

                    issueStatement = inputfile + "," + typeStr + "," + str(tmpLineNo) + "," + issueStr + "," + shortFile + "," + checkerStr + "," + str(retLineNo) + "," + statementStr + "," + functionStr + "," + causeIssueStr
                    fileIssuelist.append(issueStatement)
                    break

            lineNo += 1

            if len(lineList) and PRINT_DEBUG:
                print("=====================================")
                print(inputfile)
                print("----------------------------")
                print("  - include count: " + str(len(lineList)))
                print("  - include lines: " + str(lineList))
                print("=====================================")

    return fileIssuelist

def analyzeFilelist(inputList):
    totalIssueList = []

    for item in inputList:
        totalIssueList += analyzeFile(item)

    return totalIssueList

def writeReportFile(inputList):
    tempDir = INPUT_DIRECTORY.split("/")
    if INPUT_DIRECTORY.endswith("/"):
        dirStr = str(tempDir[len(tempDir)-2])
    else:
        dirStr = str(tempDir[len(tempDir)-1])

    reportFile = TIME_STR + "_" + dirStr + "_report.csv"

    writeListToReportFile(inputList, reportFile)

    return reportFile

def getErrorTypeAndCheckerList(inputLines):
    typeList = []
    checkerList = []

    for l in inputLines[1:]:
        tempItem = l.split(",")
        typeList.append(tempItem[3])
        checkerList.append(tempItem[5])

    typeList = set(typeList)
    typeList = list(typeList)
    checkerList = set(checkerList)
    checkerList = list(checkerList)

    return typeList, checkerList

def writeSeperatedReport(reportFile, inputType, inputLines):
    reportFileRoot = reportFile[:-4]
    reportsDirStr = reportFileRoot + "_" + SEPERTATED_REPORT_DIR

    if not os.path.isdir(reportsDirStr):
        os.mkdir(reportsDirStr)

    for type in inputType:
        seperatedReortFile = reportsDirStr + "/" + type + ".csv"
        sereratedList = []
        for l in inputLines:
            if l.find(type) != -1:
                sereratedList.append(l)

        writeListToReportFile(sereratedList, seperatedReortFile)

def writeTableHeader(inputfile):
    f = open(inputfile, 'w')

    if inputfile.endswith(".html"):
        f.write("<html>" + "\n")

    f.write("<table>" + "\n")
    f.write("    <thead>" + "\n")
    f.write("        <tr>" + "\n")
    f.write("            <th width=\"15%\">File</th>" + "\n")
    f.write("            <th width=\"5%\">Line</th>" + "\n")
    f.write("            <th>Report</th>" + "\n")
    f.write("        </tr>" + "\n")
    f.write("    </thead>" + "\n")

    f.write("    <tbody>" + "\n")
    f.close()

def writeTableFooter(inputfile):
    if os.path.isfile(inputfile):
        f = open(inputfile, 'a')
        f.write("    </tbody>" + "\n")
        f.write("</table>" + "\n")

        if inputfile.endswith(".html"):
            f.write("</html>" + "\n")
        f.close()

def writeTableContent(inputfile, checker, inputLines):
    if os.path.isfile(inputfile):
        f = open(inputfile, 'a')

        for l in inputLines[1:]:
            if l.find(checker) != -1:
                f.write("        <tr>" + "\n")
                tempItem = l.split(",")
                contentStr = "<p><font color=\"red\">" + tempItem[9] + "</font>" + "</p><p>" + tempItem[7] + "</p>"
                contentStr = contentStr.replace("\"", "")
                contentStr = contentStr.replace("_+_", ", ")
                f.write("            <td>" + tempItem[4] + "</td>" + "\n")
                f.write("            <td>" + tempItem[6] + "</td>" + "\n")
                f.write("            <td>" + contentStr + "</td>" + "\n")
                f.write("        </tr>" + "\n")

        f.close()

def writeHtmlTable(reportFile, inputChecker, inputLines):
    reportFileRoot = reportFile[:-4]
    htmlDirStr = reportFileRoot + "_" + HTML_TABLE_DIR

    if not os.path.isdir(htmlDirStr):
        os.mkdir(htmlDirStr)

    for checker in inputChecker:
        htmlTableFile = htmlDirStr + "/" + checker + ".txt"

        writeTableHeader(htmlTableFile)
        writeTableContent(htmlTableFile, checker, inputLines)
        writeTableFooter(htmlTableFile)

def makeSeperatedReports(reportFile):
    tmpFile = os.getcwd() + "/" + reportFile
    lines = readFileLines(tmpFile)

    typeList, checkerList = getErrorTypeAndCheckerList(lines)

    if PRINT_DEBUG:
        print(str(len(typeList)) + " types found")

    if MAKE_SEPERTATED_REPORT:
        writeSeperatedReport(reportFile, typeList, lines)

    if MAKE_HTML_TABLE:
        writeHtmlTable(reportFile, checkerList, lines)

def makeTimeString():
    nowDate = dt.datetime.now()
    dateStr = str(nowDate.year).zfill(4) + str(nowDate.month).zfill(2) + str(nowDate.day).zfill(2)
    timeStr = str(nowDate.hour).zfill(2) + str(nowDate.minute).zfill(2) + str(nowDate.second).zfill(2)

    return dateStr + "_" + timeStr


#================================================================================================
# Main
#================================================================================================
TIME_STR = makeTimeString()

INPUT_DIRECTORY = checkInputArg()
issueFilelist = getIssuelist()

totallist = analyzeFilelist(issueFilelist)
reportFile = writeReportFile(totallist)

makeSeperatedReports(reportFile)

print(reportFile + ": " + str(len(issueFilelist)) + " files analyzed and " + str(len(totallist)) + " issues found in " + INPUT_DIRECTORY)

#================================================================================================
# Notice
#================================================================================================
# - change " to ' in the Statement cells to process in Excel
# - change ", " to "," in the Statement cells to process in Excel
#================================================================================================