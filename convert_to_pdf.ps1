param(
    [string]$docxFile = "c:\Users\abina\Downloads\STM32 Project\Air_Monitor_PBL_Project_Report.docx",
    [string]$pdfFile  = "c:\Users\abina\Downloads\STM32 Project\Air_Monitor_PBL_Project_Report.pdf"
)

Write-Host "Opening Word Application..."
$word = New-Object -ComObject Word.Application
$word.Visible = $false
$word.DisplayAlerts = [Microsoft.Office.Interop.Word.WdAlertLevel]::wdAlertsNone

try {
    Write-Host "Opening document: $docxFile"
    $doc = $word.Documents.Open($docxFile)
    Write-Host "Converting to PDF: $pdfFile"
    $doc.SaveAs([ref]$pdfFile, [ref]17) # 17 = wdFormatPDF
    $doc.Close([ref]$false)
    Write-Host "Conversion completed successfully!"
}
catch {
    Write-Error "Error during Word to PDF conversion: $_"
}
finally {
    $word.Quit([ref]$false)
    [System.Runtime.InteropServices.Marshal]::ReleaseComObject($word) | Out-Null
    [System.GC]::Collect()
    [System.GC]::WaitForPendingFinalizers()
}
