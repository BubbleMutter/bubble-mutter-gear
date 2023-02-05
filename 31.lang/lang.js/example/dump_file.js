// server response:
// php example:
function run_php_response() {
    header('Content-Type: text/html; charset=utf-8');
    header('Content-Disposition: attachment; filename="example.txt"');
    return "file content";
}

// supports on 
// 1. 1.IE firefox
// 2. opera/google/vivaldi
// 3. safari
fetch(url, header).then(
    response => {
      const ContentDisposition = response.headers.get('Content-Disposition');
      if(ContentDisposition == null) {
        console.log('fetchPostDownload error occurs');
        console.log(response);
        return;
      }
      const filenameArray = ContentDisposition.split('filename=');
      if(filenameArray.length !== 2) {
        console.log('fetchPostDownload error occurs');
        console.log(response);
        return;
      }
      const filename = filenameArray[1];
      console.log(filename);
      response.blob().then(blob => {
          if (typeof window.navigator.msSaveBlob !== 'undefined') {
          // IE workaround for "HTML7007: One or more blob URLs were 
          // revoked by closing the blob for which they were created. 
          // These URLs will no longer resolve as the data backing 
          // the URL has been freed."
              window.navigator.msSaveBlob(blob, filename);
          } else {
             const url = window.URL.createObjectURL(blob);
             let a = document.createElement('a');
             a.href = url;
             a.style.display = 'none';
             a.setAttribute('download', filename);
        // Safari thinks _blank anchor are pop ups. We only want to set _blank
        // target if the browser does not support the HTML5 download attribute.
        // This allows you to download files in desktop safari if pop up blocking 
        // is enabled.
           if (typeof a.download === 'undefined') {
             a.setAttribute('target', '_blank');
             }
           document.body.appendChild(a);
           a.click();
           document.body.removeChild(a);
           window.URL.revokeObjectURL(url);
         }
       });
    },
    e => {
      console.log('fetchWrapper error: url = ' +
          url +
          ' header = ' +
          JSON.stringify(header));
      }
  )