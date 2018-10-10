package Mag.MagImplementation;

import java.util.List;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
/**
 * The class MagDataModel 
 *
 */
public class MagDataModel {
	private String title;
	private String lang;
	private Integer year;
	private List<String> references = null;
	private String _abstract;
	private List<String> url = null;
	private String id;
	private List<String> fos = null;

	public String getTitle() {
	return title;
	}

	public void setTitle(String title) {
	this.title = title;
	}

	public String getLang() {
	return lang;
	}

	public void setLang(String lang) {
	this.lang = lang;
	}

	public Integer getYear() {
	return year;
	}

	public void setYear(Integer year) {
	this.year = year;
	}

	public List<String> getReferences() {
	return references;
	}

	public void setReferences(List<String> references) {
	this.references = references;
	}

	public String getAbstract() {
	return _abstract;
	}

	public void setAbstract(String _abstract) {
	this._abstract = _abstract;
	}

	public List<String> getUrl() {
	return url;
	}

	public void setUrl(List<String> url) {
	this.url = url;
	}

	public String getId() {
	return id;
	}

	public void setId(String id) {
	this.id = id;
	}

	public List<String> getFos() {
	return fos;
	}

	public void setFos(List<String> fos) {
	this.fos = fos;
	}
 
	
	
}
