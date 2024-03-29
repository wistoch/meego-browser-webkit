description('Tests for tooLong flag with &lt;input> elements.');

var input = document.createElement('input');
document.body.appendChild(input);

debug('No maxlength and no value');
shouldBeFalse('input.validity.tooLong');

debug('');
debug('Non-dirty value');
input.setAttribute('value', 'abcde');
input.maxLength = 3;
shouldBe('input.value.length', '5');
shouldBeFalse('input.validity.tooLong');

input.setAttribute('value', 'abcdef');
shouldBe('input.value.length', '6');
shouldBeFalse('input.validity.tooLong');

debug('');
debug('Dirty value and longer than maxLength');
input = document.createElement('input');
document.body.appendChild(input);
input.setAttribute('value', 'abcde');
input.maxLength = 3;
input.focus();
input.setSelectionRange(5, 5);  // Move the cursor at the end.
document.execCommand('delete');
shouldBe('input.value.length', '4');
shouldBeTrue('input.validity.tooLong');
// Make the value <=maxLength.
document.execCommand('delete');
shouldBeFalse('input.validity.tooLong');

debug('');
debug('Sets a value via DOM property');
input.maxLength = 3;
input.value = 'abcde';
shouldBeTrue('input.validity.tooLong');

debug('');
debug('Disabled');
input.disabled = true;
shouldBeFalse('input.validity.tooLong');
input.disabled = false;

debug('');
debug('Grapheme length is not greater than maxLength though character length is greater');
// fancyX should be treated as 1 grapheme.
// U+0305 COMBINING OVERLINE
// U+0332 COMBINING LOW LINE
var fancyX = "x\u0305\u0332";
input = document.createElement('input');
document.body.appendChild(input);
input.value = fancyX; // 3 characters, 1 grapheme clusters.
input.maxLength = 1;
shouldBeFalse('input.validity.tooLong');

debug('');
debug('Change the type with a too long value');
input.maxLength = 3;
input.value = 'abcde';
input.type = 'search';
shouldBeTrue('input.validity.tooLong');

input.type = 'number';
shouldBeFalse('input.validity.tooLong');

var successfullyParsed = true;
